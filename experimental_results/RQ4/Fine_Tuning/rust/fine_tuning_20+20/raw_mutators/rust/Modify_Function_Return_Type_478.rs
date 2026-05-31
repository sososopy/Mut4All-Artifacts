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

pub struct Modify_Function_Return_Type_478;

impl Mutator for Modify_Function_Return_Type_478 {
    fn name(&self) -> &str {
        "Modify_Function_Return_Type_478"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct TypeAliasCollector {
            type_aliases: HashSet<Ident>,
        }

        impl<'ast> Visit<'ast> for TypeAliasCollector {
            fn visit_item_type(&mut self, item_type: &'ast syn::ItemType) {
                self.type_aliases.insert(item_type.ident.clone());
            }
        }

        let mut collector = TypeAliasCollector {
            type_aliases: HashSet::new(),
        };
        collector.visit_file(file);

        struct TypeAliasReplacer<'a> {
            type_aliases: &'a HashSet<Ident>,
        }

        impl<'a> VisitMut for TypeAliasReplacer<'a> {
            fn visit_item_fn_mut(&mut self, item_fn: &mut syn::ItemFn) {
                if let ReturnType::Type(_, return_type) = &mut item_fn.sig.output {
                    if let Type::Path(type_path) = &**return_type {
                        if type_path.qself.is_none()
                            && type_path
                                .path
                                .get_ident()
                                .map_or(false, |ident| self.type_aliases.contains(ident))
                        {
                            *return_type = Box::new(Type::Verbatim(quote! { impl Sized }));
                        }
                    }
                }
                syn::visit_mut::visit_item_fn_mut(self, item_fn);
            }
        }

        let mut replacer = TypeAliasReplacer {
            type_aliases: &collector.type_aliases,
        };
        replacer.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets functions that return a type alias for an `impl Trait`. It replaces the return type with a direct `impl Trait` specification, specifically `impl Sized`. This transformation stresses the compiler's type resolution and trait inference mechanisms by altering the return type's abstraction layer, potentially uncovering bugs related to opaque type handling and alias resolution."
    }
}