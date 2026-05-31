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

pub struct Add_Specialized_Trait_Impl_For_TAIT_258;

impl Mutator for Add_Specialized_Trait_Impl_For_TAIT_258 {
    fn name(&self) -> &str {
        "Add_Specialized_Trait_Impl_For_TAIT_258"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut trait_name = None;
        let mut blanket_impl = None;
        let mut tait_alias = None;
        let mut tait_position = None;

        for item in &mut file.items {
            if let Item::Trait(item_trait) = item {
                trait_name = Some(item_trait.ident.clone());
            }
            if let Item::Impl(item_impl) = item {
                if let Some(trait_path) = &item_impl.trait_ {
                    if trait_path.1.segments.last().map(|seg| seg.ident.clone()) == trait_name.clone() {
                        if item_impl.generics.params.len() > 0 {
                            blanket_impl = Some(item_impl.clone());
                        }
                    }
                }
            }
            if let Item::Type(item_type) = item {
                if let Type::ImplTrait(_) = &*item_type.ty {
                    tait_alias = Some(item_type.ident.clone());
                    tait_position = Some(item.clone());
                }
            }
        }

        if let (Some(trait_name), Some(blanket_impl), Some(tait_alias), Some(tait_position)) = (trait_name, blanket_impl, tait_alias, tait_position) {
            let mut new_impl = blanket_impl.clone();
            new_impl.generics.params.clear();
            new_impl.generics.where_clause = None;
            new_impl.trait_ = Some((None, syn::Path::from(trait_name.clone())));
            new_impl.self_ty = Box::new(Type::Path(TypePath {
                qself: None,
                path: syn::Path::from(tait_alias.clone()),
            }));

            for impl_item in &mut new_impl.items {
                if let ImplItem::Fn(method) = impl_item {
                    method.sig.generics.params.clear();
                    method.sig.generics.where_clause = None;
                    method.defaultness = Some(syn::token::Default::default());
                }
                if let ImplItem::Const(const_item) = impl_item {
                    const_item.defaultness = Some(syn::token::Default::default());
                }
                if let ImplItem::Type(type_item) = impl_item {
                    type_item.defaultness = Some(syn::token::Default::default());
                }
                if let ImplItem::Macro(macro_item) = impl_item {
                    macro_item.defaultness = Some(syn::token::Default::default());
                }
            }

            let mut new_items = Vec::new();
            for item in &mut file.items {
                new_items.push(item.clone());
                if let Item::Impl(item_impl) = item {
                    if item_impl == blanket_impl {
                        new_items.push(Item::Impl(new_impl));
                    }
                }
            }
            file.items = new_items;
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies trait definitions, blanket implementations, and type alias impl Trait (TAIT) definitions in the seed program. It creates a specialized implementation of the trait for the TAIT, copying the method signatures from the blanket impl and marking them as default. This introduces a specialization scenario that may trigger compiler bugs related to TAIT coherence and specialization handling."
    }
}