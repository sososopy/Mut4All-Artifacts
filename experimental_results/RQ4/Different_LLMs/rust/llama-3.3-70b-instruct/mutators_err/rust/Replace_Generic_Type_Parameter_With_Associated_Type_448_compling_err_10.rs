use syn::{GenericParam, TypeParamBound, Ident, FnArg, Type};
use crate::mutator::Mutator;

struct Replace_Generic_Type_Parameter_With_Associated_Type_448;

impl Mutator for Replace_Generic_Type_Parameter_With_Associated_Type_448 {
    fn name(&self) -> &str {
        "Replace_Generic_Type_Parameter_With_Associated_Type_448"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                if let Some(generics) = &mut trait_item.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Type(type_param) = param {
                            if let Some(trait_bounds) = &type_param.bounds {
                                for bound in trait_bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        if let Some(path) = trait_bound.path.get_ident() {
                                            if let Some(associated_type) = get_associated_type(trait_item, path) {
                                                type_param.ident = associated_type;
                                                type_param.bounds = Default::default();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if let syn::Item::Impl(impl_item) = item {
                if let Some(generics) = &mut impl_item.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Type(type_param) = param {
                            if let Some(trait_bounds) = &type_param.bounds {
                                for bound in trait_bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        if let Some(path) = trait_bound.path.get_ident() {
                                            if let Some(associated_type) = get_associated_type_from_impl(impl_item, path) {
                                                type_param.ident = associated_type;
                                                type_param.bounds = Default::default();
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if let syn::Item::Fn(func) = item {
                if let Some(generics) = &mut func.sig.generics {
                    for param in &mut generics.params {
                        if let GenericParam::Type(type_param) = param {
                            if let Some(trait_bounds) = &type_param.bounds {
                                for bound in trait_bounds {
                                    if let TypeParamBound::Trait(trait_bound) = bound {
                                        if let Some(path) = trait_bound.path.get_ident() {
                                            if let Some(associated_type) = get_associated_type_from_fn(func, path) {
                                                type_param.ident = associated_type;
                                                type_param.bounds = Default::default();
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
    }

    fn chain_of_thought(&self) -> &str {
        "The mutation operator replaces generic type parameters with associated types of the same trait. This transformation aims to test the compiler's handling of complex type relationships and associated types, potentially triggering bugs related to non-lifetime binders."
    }
}

fn get_associated_type(trait_item: &syn::ItemTrait, path: &Ident) -> Option<Ident> {
    for item in &trait_item.items {
        if let syn::TraitItem::Type(associated_type) = item {
            if associated_type.ident == *path {
                return Some(associated_type.ident.clone());
            }
        }
    }
    None
}

fn get_associated_type_from_impl(impl_item: &syn::ItemImpl, path: &Ident) -> Option<Ident> {
    for item in &impl_item.items {
        if let syn::ImplItem::Type(associated_type) = item {
            if associated_type.ident == *path {
                return Some(associated_type.ident.clone());
            }
        }
    }
    None
}

fn get_associated_type_from_fn(func: &syn::ItemFn, path: &Ident) -> Option<Ident> {
    for input in &func.sig.inputs {
        if let FnArg::Typed(pat_type) = input {
            if let Type::Path(type_path) = &*pat_type.ty {
                if let Some(segment) = type_path.path.segments.last() {
                    if segment.ident == *path {
                        return Some(segment.ident.clone());
                    }
                }
            }
        }
    }
    None
}