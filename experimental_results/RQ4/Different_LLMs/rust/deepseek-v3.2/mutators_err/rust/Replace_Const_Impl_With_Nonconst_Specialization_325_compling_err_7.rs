use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait2,
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

pub struct Replace_Const_Impl_With_Nonconst_Specialization_325;

impl Mutator for Replace_Const_Impl_With_Nonconst_Specialization_325 {
    fn name(&self) -> &str {
        "Replace_Const_Impl_With_Nonconst_Specialization_325"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut const_trait_impls = Vec::new();
        let mut trait_names = HashSet::new();
        for item in &file.items {
            if let Item::Trait(item_trait) = item {
                for attr in &item_trait.attrs {
                    if attr.path().is_ident("const_trait") {
                        trait_names.insert(item_trait.ident.clone());
                        break;
                    }
                }
            }
        }
        for item in &mut file.items {
            if let Item::Impl(item_impl) = item {
                if let Some((_, trait_path, _)) = &item_impl.trait_ {
                    if trait_names.contains(&trait_path.segments.last().unwrap().ident) {
                        if item_impl.defaultness.is_some() {
                            const_trait_impls.push(item_impl);
                        }
                    }
                }
            }
        }
        if const_trait_impls.len() < 2 {
            return;
        }
        let mut rng = thread_rng();
        const_trait_impls.shuffle(&mut rng);
        for i in 0..const_trait_impls.len() {
            for j in 0..const_trait_impls.len() {
                if i == j {
                    continue;
                }
                let impl_a = &const_trait_impls[i];
                let impl_b = &const_trait_impls[j];
                let type_a = &impl_a.self_ty;
                let type_b = &impl_b.self_ty;
                let mut is_more_specific = false;
                if let Type::Path(type_path_a) = &**type_a {
                    if let Type::Path(type_path_b) = &**type_b {
                        let segs_a = &type_path_a.path.segments;
                        let segs_b = &type_path_b.path.segments;
                        if segs_a.len() == segs_b.len() {
                            let mut generic_mismatch = false;
                            for (seg_a, seg_b) in segs_a.iter().zip(segs_b.iter()) {
                                match (&seg_a.arguments, &seg_b.arguments) {
                                    (PathArguments::AngleBracketed(args_a), PathArguments::AngleBracketed(args_b)) => {
                                        if args_a.args.len() > 0 && args_b.args.len() == 0 {
                                            is_more_specific = true;
                                            break;
                                        } else if args_a.args.len() == 0 && args_b.args.len() > 0 {
                                            generic_mismatch = true;
                                            break;
                                        }
                                    }
                                    (PathArguments::None, PathArguments::AngleBracketed(_)) => {
                                        is_more_specific = true;
                                        break;
                                    }
                                    _ => {}
                                }
                            }
                            if generic_mismatch {
                                continue;
                            }
                        }
                    }
                }
                if is_more_specific {
                    impl_b.defaultness = None;
                    return;
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies pairs of const trait implementations where one is more specific than the other (e.g., generic vs concrete). It then removes the const keyword from the more specific impl, creating a non-const impl that attempts to specialize a const impl. This transformation targets edge cases in the compiler's const trait and specialization interaction, potentially triggering internal compiler errors about parent items not being marked as default."
    }
}