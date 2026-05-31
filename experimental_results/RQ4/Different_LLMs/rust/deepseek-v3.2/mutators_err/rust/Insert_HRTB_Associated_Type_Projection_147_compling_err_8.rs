use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath, parse_quote,
    punctuated::Punctuated,
    spanned::Spanned,
    token,
    token::Comma,
    token::{Paren, Plus},
    visit::Visit,
    visit_mut::VisitMut,
    *,
};

use crate::mutator::Mutator;

pub struct Insert_HRTB_Associated_Type_Projection_147;

impl Mutator for Insert_HRTB_Associated_Type_Projection_147 {
    fn name(&self) -> &str {
        "Insert_HRTB_Associated_Type_Projection_147"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_names = HashSet::new();
        for item in &file.items {
            if let Item::Trait(trait_item) = item {
                trait_names.insert(trait_item.ident.to_string());
            }
        }
        
        let target_trait_name = if trait_names.is_empty() {
            "Trait"
        } else {
            trait_names.iter().next().unwrap()
        };
        
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                self.mutate_fn_signature(&mut item_fn.sig, target_trait_name);
            }
            if let Item::Impl(item_impl) = item {
                for impl_item in &mut item_impl.items {
                    if let ImplItem::Fn(func) = impl_item {
                        self.mutate_fn_signature(&mut func.sig, target_trait_name);
                    }
                }
            }
            if let Item::Trait(item_trait) = item {
                for trait_item in &mut item_trait.items {
                    if let TraitItem::Fn(method) = trait_item {
                        self.mutate_fn_signature(&mut method.sig, target_trait_name);
                    }
                }
            }
            if let Item::Type(item_type) = item {
                self.mutate_type_alias(&mut item_type.ty, target_trait_name);
            }
        }
        
        if trait_names.is_empty() {
            file.items.push(parse_quote! {
                trait Trait {}
            });
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator inserts higher-ranked trait bounds (HRTB) with erroneous associated type projections in trait bounds. It targets function signatures, trait method signatures, and type alias definitions. When encountering a trait bound referencing a trait with zero generic parameters, it replaces the bound with a `for<T>` HRTB that adds an associated type projection using a generic parameter `T` that doesn't exist in the trait definition. This creates DefId resolution failures during bound variable analysis, testing the compiler's handling of mismatched generic parameters in HRTB contexts and associated type specifications."
    }
}

impl Insert_HRTB_Associated_Type_Projection_147 {
    fn mutate_fn_signature(&self, sig: &mut Signature, trait_name: &str) {
        for param in &mut sig.generics.params {
            if let GenericParam::Type(type_param) = param {
                let mut new_bounds = Punctuated::new();
                for bound in &type_param.bounds {
                    if let TypeParamBound::Trait(trait_bound) = bound {
                        if trait_bound.path.segments.len() == 1 && trait_bound.path.segments[0].ident == trait_name {
                            let new_bound = self.create_hrtb_bound(trait_name);
                            new_bounds.push(TypeParamBound::Trait(new_bound));
                        } else {
                            new_bounds.push(bound.clone());
                        }
                    } else {
                        new_bounds.push(bound.clone());
                    }
                }
                type_param.bounds = new_bounds;
            }
        }
        
        if let Some(where_clause) = &mut sig.generics.where_clause {
            for predicate in &mut where_clause.predicates {
                if let WherePredicate::Type(pred_type) = predicate {
                    if let Type::Path(type_path) = &pred_type.bounded_ty {
                        if type_path.path.segments.len() == 1 && type_path.path.segments[0].ident == trait_name {
                            let new_bound = self.create_hrtb_bound(trait_name);
                            pred_type.bounds.push(TypeParamBound::Trait(new_bound));
                        }
                    }
                }
            }
        }
    }
    
    fn mutate_type_alias(&self, ty: &mut Type, trait_name: &str) {
        if let Type::Path(type_path) = ty {
            if type_path.path.segments.len() == 1 && type_path.path.segments[0].ident == trait_name {
                let new_bound = self.create_hrtb_bound(trait_name);
                let new_type = parse_quote! {
                    for<T> #trait_name<'_, Assoc = impl #trait_name<T> + '_>
                };
                *ty = new_type;
            }
        }
    }
    
    fn create_hrtb_bound(&self, trait_name: &str) -> TraitBound {
        let trait_path = SynPath::from(Ident::new(trait_name, Span::call_site()));
        
        let hrtb_bound = parse_quote! {
            for<T> #trait_path<'_, Assoc = impl #trait_path<T> + '_>
        };
        
        match hrtb_bound {
            Type::TraitObject(obj) => {
                if let Some(TypeParamBound::Trait(bound)) = obj.bounds.first() {
                    bound.clone()
                } else {
                    TraitBound {
                        paren_token: None,
                        modifier: TraitBoundModifier::None,
                        lifetimes: Some(BoundLifetimes {
                            lifetimes: Punctuated::from_iter(vec![
                                GenericParam::Lifetime(LifetimeParam::new(Lifetime::new("'a", Span::call_site())))
                            ]),
                            for_token: token::For::default(),
                            lt_token: token::Lt::default(),
                            gt_token: token::Gt::default(),
                        }),
                        path: trait_path,
                    }
                }
            },
            _ => TraitBound {
                paren_token: None,
                modifier: TraitBoundModifier::None,
                lifetimes: Some(BoundLifetimes {
                    lifetimes: Punctuated::from_iter(vec![
                        GenericParam::Lifetime(LifetimeParam::new(Lifetime::new("'a", Span::call_site())))
                    ]),
                    for_token: token::For::default(),
                    lt_token: token::Lt::default(),
                    gt_token: token::Gt::default(),
                }),
                path: trait_path,
            }
        }
    }
}