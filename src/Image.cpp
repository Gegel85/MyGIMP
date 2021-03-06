/*
** EPITECH PROJECT, 2020
** MyGimp
** File description:
** Image.cpp
*/
#include <SFML/Graphics/Image.hpp>
#include "Image.hpp"
#include "Exceptions.hpp"
#include "Snapshot/FrameBufferSnapshot.hpp"
#include "Snapshot/LayerSnapshot.hpp"
#include "Snapshot/SelectionSnapshot.hpp"
#include "Editor.hpp"
#include "Snapshot/LayerManagerSnapshot.hpp"

namespace Mimp {
	Layer &Image::getSelectedLayer() noexcept
	{
		return this->_layers->getSelectedLayer();
	}

	const Layer &Image::getSelectedLayer() const noexcept
	{
		return this->_layers->getSelectedLayer();
	}

	Image::Image(Vector2<unsigned> size):
		_size(size),
		_layers(std::make_shared<LayerManager>(size)),
		selectedArea(std::make_shared<SelectedArea>(size))
	{}

	Image::Image(Vector2<unsigned> size, const LayerManager &layers):
		_size(size),
		_layers(std::make_shared<LayerManager>(layers)),
		selectedArea(std::make_shared<SelectedArea>(size))
	{
	}

	LayerManager &Image::getLayers() noexcept
	{
		return *this->_layers;
	}

	const LayerManager &Image::getLayers() const noexcept
	{
		return *this->_layers;
	}

	Vector2<unsigned> Image::getImageSize() const noexcept
	{
		return this->_size;
	}

	void Image::exportImage(const std::string &path) const
	{
		FrameBuffer buffer({
			static_cast<unsigned>(this->getImageSize().x),
			static_cast<unsigned>(this->getImageSize().y)
		});
		auto size = buffer.getSize();
		sf::Image image;
		auto pixelArray = new sf::Color[size.x * size.y];

		this->_layers->render(buffer);
		for (unsigned x = 0; x < size.x; x++)
			for (unsigned y = 0; y < size.y; y++)
				pixelArray[x + y * size.x] = buffer.getPixel({static_cast<int>(x), static_cast<int>(y)});

		image.create(size.x, size.y, reinterpret_cast<const sf::Uint8 *>(pixelArray));
		delete[] pixelArray;
		if (!image.saveToFile(path))
			throw ExportErrorException("Cannot export to file " + path);
	}

	unsigned int Image::getMaxSnapshots() const noexcept
	{
		return this->_max_snapshots;
	}

	void Image::setMaxSnapshots(unsigned int maxSnapshots) noexcept
	{
		this->_max_snapshots = maxSnapshots;
	}

	void Image::takeSnapshot(std::shared_ptr<Snapshot> snapshot) noexcept
	{
		this->_edited = true;
		this->_snapshots.push_back(snapshot);
		if (this->_snapshots.size() > this->_max_snapshots)
			this->_snapshots.erase(this->_snapshots.begin());
		this->_redoSnapshots.clear();
	}

	void Image::takeFrameBufferSnapshot() noexcept
	{
		this->takeSnapshot(std::make_shared<FrameBufferSnapshot>(
			*this->getSelectedLayer().buffer, this->_layers->getSelectedLayerIndex()
		));
	}

	void Image::takeLayerSnapshot() noexcept
	{
		this->takeLayerSnapshot(this->_layers->getSelectedLayerIndex());
	}

	void Image::takeLayerSnapshot(unsigned index) noexcept
	{
		this->takeSnapshot(std::make_shared<LayerSnapshot>(
			this->getLayers()[index], index
		));
	}

	void Image::takeSelectionSnapshot() noexcept
	{
		this->takeSnapshot(std::make_shared<SelectionSnapshot>(
			*this->selectedArea
		));
	}

	void Image::takeLayerManagerSnapshot() noexcept
	{
		this->takeSnapshot(std::make_shared<LayerManagerSnapshot>(
				*this->_layers
		));
	}

	void Image::undoLastAction(Editor &editor) noexcept
	{
		if (this->_snapshots.empty())
			return;
		std::shared_ptr<Snapshot> snapshot = this->_snapshots.back();
		snapshot->undo(*this, editor);
		this->_redoSnapshots.emplace_back(snapshot);
		this->_snapshots.pop_back();
	}

	void Image::redoLastUndo(Editor &editor) noexcept
	{
		if (this->_redoSnapshots.empty())
			return;
		std::shared_ptr<Snapshot> snapshot = this->_redoSnapshots.back();
		snapshot->redo(*this, editor);
		this->_snapshots.emplace_back(snapshot);
		this->_redoSnapshots.pop_back();
	}

	std::shared_ptr<LayerManager> &Image::getLayerManagerPtr() noexcept
	{
		return this->_layers;
	}

	bool Image::isEdited() const
	{
		return this->_edited;
	}

	void Image::setEdited(bool edited)
	{
		this->_edited = edited;
	}
}