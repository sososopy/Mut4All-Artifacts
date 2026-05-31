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

pub struct Modify_Opaque_Type_Usage_In_Function_Signatures_279;

impl Mutator for Modify_Opaque_Type_Usage_In_Function_Signatures_279 {
    fn name(&self) -> &str {
        "Modify_Opaque_Type_Usage_In_Function_Signatures_279"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut opaque_aliases = HashSet::new();
        for item in &file.items {
            if let Item::Type(item_type) = item {
                if let Type::ImplTrait(_) = &*item_type.ty {
                    opaque_aliases.insert(item_type.ident.to_string());
                }
            }
        }
        struct OpaqueAliasReplacer<'a> {
            aliases: &'a HashSet<String>,
        }
        impl<'a> VisitMut for OpaqueAliasReplacer<'a> {
            fn visit_type_mut(&mut self, node: &mut Type) {
                if let Type::Path(type_path) = node {
                    if type_path.qself.is_none()
                        && type_path
                            .path
                            .get_ident()
                            .map(|ident| self.aliases.contains(&ident.to_string()))
                            .unwrap_or(false)
                    {
                        *node = Type::Infer(TypeInfer {
                            underscore_token: Default::default(),
                        });
                        return;
                    }
                }
                syn::visit_mut::visit_type_mut(self, node);
            }
        }
        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                let mut has_opaque_alias = false;
                for input in &item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Path(type_path) = &*pat_type.ty {
                            if type_path.qself.is_none()
                                && type_path
                                    .path
                                    .get_ident()
                                    .map(|ident| opaque_aliases.contains(&ident.to_string()))
                                    .unwrap_or(false)
                            {
                                has_opaque_alias = true;
                                break;
                            }
                        }
                    }
                }
                if !has_opaque_alias {
                    continue;
                }
                let mut replacer = OpaqueAliasReplacer {
                    aliases: &opaque_aliases,
                };
                for input in &mut item_fn.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        replacer.visit_type_mut(&mut pat_type.ty);
                    }
                }
                if let ReturnType::Type(_, ty) = &mut item_fn.sig.output {
                    replacer.visit_type_mut(ty);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions that use opaque type aliases (e.g., `type Foo = impl Trait;`) in their signatures. It replaces occurrences of these aliases with type inference placeholders (`_`) in both parameter and return types. This transformation forces the compiler to infer the opaque type directly, potentially revealing bugs in type inference and opaque type resolution."
    }
}