use crate::mutator::Mutator;
use syn::{Item, ItemTrait, TraitItem, TypeParamBound, TraitBoundModifier};

pub struct Remove_Const_Assoc_Bound_327;

impl Mutator for Remove_Const_Assoc_Bound_327 {
    fn name(&self) -> &str {
        "Remove_Const_Assoc_Bound_327"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Trait(trait_item) = item {
                if trait_item.attrs.iter().any(|attr| attr.path().is_ident("const_trait")) {
                    for trait_item in &mut trait_item.items {
                        if let TraitItem::Type(ty) = trait_item {
                            for bound in &mut ty.bounds {
                                if let TypeParamBound::Trait(trait_bound) = bound {
                                    if let TraitBoundModifier::ForAuto(expr) = &trait_bound.modifier {
                                        if let syn::Expr::Lit(_) = expr.as_ref() {
                                            trait_bound.modifier = TraitBoundModifier::None;
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
        ""
    }
}