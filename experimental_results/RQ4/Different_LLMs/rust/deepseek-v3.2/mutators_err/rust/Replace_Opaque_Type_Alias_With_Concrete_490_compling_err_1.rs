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

pub struct Replace_Opaque_Type_Alias_With_Concrete_490;

impl Mutator for Replace_Opaque_Type_Alias_With_Concrete_490 {
    fn name(&self) -> &str {
        "Replace_Opaque_Type_Alias_With_Concrete_490"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut opaque_aliases = Vec::new();
        for item in &file.items {
            if let Item::Type(item_type) = item {
                if let Type::ImplTrait(_) = &item_type.ty {
                    opaque_aliases.push(item_type.ident.clone());
                }
            }
        }

        if opaque_aliases.is_empty() {
            return;
        }

        for item in &mut file.items {
            if let Item::Fn(item_fn) = item {
                if let ReturnType::Type(_, return_type) = &item_fn.sig.output {
                    let mut visitor = OpaqueReplacer {
                        opaque_aliases: &opaque_aliases,
                        return_type: return_type,
                        changed: false,
                    };
                    visitor.visit_block_mut(&mut item_fn.block);
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets functions returning opaque type aliases and replaces opaque type usage in variable declarations with concrete types. It analyzes the return type structure to infer appropriate concrete replacements, preferring wrapper inner types or unit types. This transformation creates type mismatches that test compiler handling of opaque vs concrete type substitution and trait bound satisfaction."
    }
}

struct OpaqueReplacer<'a> {
    opaque_aliases: &'a Vec<Ident>,
    return_type: &'a Box<Type>,
    changed: bool,
}

impl<'a> VisitMut for OpaqueReplacer<'a> {
    fn visit_local_mut(&mut self, local: &mut Local) {
        if let Some(init) = &local.init {
            if let Pat::Type(pat_type) = &mut local.pat {
                if let Type::Path(type_path) = &mut *pat_type.ty {
                    if type_path.path.segments.len() == 1 {
                        let segment = &type_path.path.segments[0];
                        if self.opaque_aliases.contains(&segment.ident) {
                            let concrete_type = self.infer_concrete_type();
                            pat_type.ty = Box::new(concrete_type);
                            self.changed = true;
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_local_mut(self, local);
    }
}

impl<'a> OpaqueReplacer<'a> {
    fn infer_concrete_type(&self) -> Type {
        let mut visitor = TypeAnalyzer {
            found_type: None,
            in_option: false,
        };
        visitor.visit_type(&self.return_type);

        if let Some(found) = visitor.found_type {
            return found;
        }

        if visitor.in_option {
            return parse_quote! { () };
        }

        parse_quote! { () }
    }
}

struct TypeAnalyzer {
    found_type: Option<Type>,
    in_option: bool,
}

impl Visit for TypeAnalyzer {
    fn visit_type_path(&mut self, node: &TypePath) {
        if node.path.segments.len() == 1 {
            let segment = &node.path.segments[0];
            if segment.ident == "Option" {
                self.in_option = true;
                if let PathArguments::AngleBracketed(args) = &segment.arguments {
                    for arg in &args.args {
                        if let GenericArgument::Type(ty) = arg {
                            self.visit_type(ty);
                        }
                    }
                }
                return;
            }
        }

        if self.found_type.is_none() && !self.in_option {
            self.found_type = Some(Type::Path(node.clone()));
        }
        syn::visit::visit_type_path(self, node);
    }

    fn visit_type(&mut self, node: &Type) {
        match node {
            Type::ImplTrait(_) => {}
            _ => syn::visit::visit_type(self, node),
        }
    }
}