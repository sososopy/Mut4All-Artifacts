use crate::mutator::Mutator;
use syn::{Item, ReturnType, Type, Ident, TypePath, token, punctuated::Punctuated, TraitItem, TraitItemType, Generics, Path};
use proc_macro2::Span;

pub struct Trait_AssocType_Cycle_446;

impl Mutator for Trait_AssocType_Cycle_446 {
    fn name(&self) -> &str {
        "Trait_AssocType_Cycle_446"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut assoc_type_items = Vec::new();
        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                let method_ident = item_trait.items.iter().find_map(|i| {
                    if let TraitItem::Fn(m) = i {
                        if let ReturnType::Type(_, ty) = &m.sig.output {
                            if let Type::ImplTrait(_) = **ty {
                                return Some(m.sig.ident.clone());
                            }
                        }
                    }
                    None
                });

                if let Some(method_ident) = method_ident {
                    let assoc_type_ident = Ident::new("AssocType", Span::call_site());
                    let mut bounds = Punctuated::new();
                    if let Some(TraitItem::Fn(method)) = item_trait.items.iter().find(|i| {
                        if let TraitItem::Fn(m) = i {
                            return m.sig.ident == method_ident;
                        }
                        false
                    }) {
                        if let ReturnType::Type(_, ty) = &method.sig.output {
                            if let Type::ImplTrait(impl_trait) = &**ty {
                                for bound in &impl_trait.bounds {
                                    bounds.push(bound.clone());
                                }
                            }
                        }
                    }

                    assoc_type_items.push(TraitItem::Type(TraitItemType {
                        attrs: vec![],
                        type_token: token::Type(Span::call_site()),
                        ident: assoc_type_ident.clone(),
                        generics: Generics::default(),
                        colon_token: Some(token::Colon(Span::call_site())),
                        bounds,
                        default: None,
                        semi_token: token::Semi(Span::call_site()),
                    }));

                    if let Some(TraitItem::Fn(method)) = item_trait.items.iter_mut().find(|i| {
                        if let TraitItem::Fn(m) = i {
                            return m.sig.ident == method_ident;
                        }
                        false
                    }) {
                        method.sig.output = ReturnType::Type(
                            token::RArrow(Span::call_site()),
                            Box::new(Type::Path(TypePath {
                                qself: None,
                                path: Path::from(assoc_type_ident),
                            })),
                        );
                    }
                }
            }
        }
        for item in assoc_type_items {
            if let Some(Item::Trait(item_trait)) = file.items.iter_mut().find(|i| matches!(i, Item::Trait(_))) {
                item_trait.items.push(item);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutation targets a trait definition that includes a method returning an `impl Trait`. It modifies the trait by introducing an associated type `AssocType` with the same constraints as the method's return type. The method's return type is then changed to `Self::AssocType`. This can create conflicts if the trait is implemented for a type that cannot satisfy the associated type's bounds, thus exploring potential inconsistencies in Rust's handling of associated types and `impl Trait` return types."
    }
}