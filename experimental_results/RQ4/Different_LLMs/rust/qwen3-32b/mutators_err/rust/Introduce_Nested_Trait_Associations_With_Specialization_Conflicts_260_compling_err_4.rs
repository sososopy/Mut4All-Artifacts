use syn::parse_quote;
use crate::mutator::Mutator;

pub struct Introduce_Nested_Trait_Associations_With_Specialization_Conflicts_260;

impl Mutator for Introduce_Nested_Trait_Associations_With_Specialization_Conflicts_260 {
    fn name(&self) -> &str {
        "Introduce_Nested_Trait_Associations_With_Specialization_Conflicts_260"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Trait(trait_item) = item {
                let has_output = trait_item.items.iter().any(|item| {
                    if let syn::TraitItem::Type(ty) = item {
                        ty.ident == "Output"
                    } else {
                        false
                    }
                });
                if has_output {
                    let trait_name = &trait_item.ident;
                    let mut impls: Vec<_> = file.items.iter_mut()
                        .filter_map(|i| {
                            if let syn::Item::Impl(impl_item) = i {
                                if let Some((_, ref path, _)) = impl_item.trait_ {
                                    if path.is_ident(trait_name) {
                                        return Some(impl_item);
                                    }
                                }
                                None
                            } else {
                                None
                            }
                        })
                        .collect();
                    if !impls.is_empty() {
                        let specific_impl = impls.iter().find(|impl_item| {
                            if let Some((_, ref path, _)) = impl_item.trait_ {
                                if path.is_ident(trait_name) {
                                    if let syn::Type::Path(type_path) = &*impl_item.self_ty {
                                        if let Some(segment) = type_path.path.segments.last() {
                                            if !segment.arguments.is_empty() {
                                                return false;
                                            }
                                            return true;
                                        }
                                    }
                                }
                            }
                            false
                        });
                        if let Some(impl_item) = specific_impl {
                            if let syn::Type::Path(type_path) = &*impl_item.self_ty {
                                if let Some(segment) = type_path.path.segments.last() {
                                    let t_type = segment.ident.clone();
                                    let new_trait = parse_quote! {
                                        trait OtherTrait {
                                            type Nested;
                                        }
                                    };
                                    file.items.push(syn::Item::Trait(new_trait));
                                    let assoc_path: syn::Path = parse_quote! { #trait_name };
                                    let nested_path = parse_quote! { <#t_type as #assoc_path>::Output };
                                    let impl_other = parse_quote! {
                                        impl OtherTrait for #nested_path {
                                            type Nested = <Self as #assoc_path>::Output;
                                        }
                                    };
                                    file.items.push(syn::Item::Impl(impl_other));
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        ""
    }
}