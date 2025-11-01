use syn::{Item, PathArguments, GenericArgument, TypeParamBound};
use proc_macro2::{Ident, Span};

pub struct Modify_Trait_Bound_375;

impl Mutator for Modify_Trait_Bound_375 {
    fn name(&self) -> &str {
        "Modify_Trait_Bound_375"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, path, _)) = &mut item_impl.trait_ {
                    if let Some(last_segment) = path.segments.last_mut() {
                        if let PathArguments::AngleBracketed(ref mut args) = last_segment.arguments {
                            for arg in &mut args.args {
                                if let GenericArgument::Type(ref mut ty) = arg {
                                    if let syn::Type::Path(ref mut type_path) = ty {
                                        if let Some(last_segment) = type_path.path.segments.last_mut() {
                                            last_segment.ident = Ident::new("NonExistentTrait", Span::call_site());
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets trait implementations with specified type bounds. It modifies the trait bound to introduce a mismatch, replacing it with an incompatible or non-existent trait. This challenges the validity of type constraints, potentially causing trait resolution issues."
    }
}