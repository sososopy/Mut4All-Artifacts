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

pub struct Modify_Generic_Arguments_In_Struct_Instantiation_233;

impl Mutator for Modify_Generic_Arguments_In_Struct_Instantiation_233 {
    fn name(&self) -> &str {
        "Modify_Generic_Arguments_In_Struct_Instantiation_233"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct StructInstantiationFinder {
            candidates: Vec<(usize, usize)>,
        }

        impl<'ast> Visit<'ast> for StructInstantiationFinder {
            fn visit_expr(&mut self, node: &'ast Expr) {
                if let Expr::Struct(expr_struct) = node {
                    if let Some(path_segment) = expr_struct.path.segments.last() {
                        if let PathArguments::AngleBracketed(arguments) =
                            &path_segment.arguments
                        {
                            let num_generics = arguments.args.len();
                            self.candidates
                                .push((expr_struct.span().start().line, num_generics));
                        }
                    }
                }
                syn::visit::visit_expr(self, node);
            }
        }

        let mut finder = StructInstantiationFinder {
            candidates: Vec::new(),
        };
        finder.visit_file(file);

        let mut rng = thread_rng();
        finder.candidates.shuffle(&mut rng);

        if let Some((target_line, num_generics)) = finder.candidates.first() {
            let mut existing_types = HashSet::new();
            let mut existing_lifetimes = HashSet::new();

            for item in &file.items {
                if let Item::Struct(item_struct) = item {
                    existing_types.insert(item_struct.ident.to_string());
                }
                if let Item::Enum(item_enum) = item {
                    existing_types.insert(item_enum.ident.to_string());
                }
                if let Item::Type(item_type) = item {
                    existing_types.insert(item_type.ident.to_string());
                }
                if let Item::Trait(item_trait) = item {
                    existing_types.insert(item_trait.ident.to_string());
                }
                if let Item::Impl(item_impl) = item {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        if let Some(segment) = path.segments.last() {
                            existing_types.insert(segment.ident.to_string());
                        }
                    }
                }
                if let Item::Struct(item_struct) = item {
                    for param in &item_struct.generics.params {
                        if let GenericParam::Type(type_param) = param {
                            existing_types.insert(type_param.ident.to_string());
                        }
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            existing_lifetimes.insert(lifetime_param.lifetime.ident.to_string());
                        }
                    }
                }
                if let Item::Enum(item_enum) = item {
                    for param in &item_enum.generics.params {
                        if let GenericParam::Type(type_param) = param {
                            existing_types.insert(type_param.ident.to_string());
                        }
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            existing_lifetimes.insert(lifetime_param.lifetime.ident.to_string());
                        }
                    }
                }
                if let Item::Fn(item_fn) = item {
                    for param in &item_fn.sig.generics.params {
                        if let GenericParam::Type(type_param) = param {
                            existing_types.insert(type_param.ident.to_string());
                        }
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            existing_lifetimes.insert(lifetime_param.lifetime.ident.to_string());
                        }
                    }
                }
                if let Item::Impl(item_impl) = item {
                    for param in &item_impl.generics.params {
                        if let GenericParam::Type(type_param) = param {
                            existing_types.insert(type_param.ident.to_string());
                        }
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            existing_lifetimes.insert(lifetime_param.lifetime.ident.to_string());
                        }
                    }
                }
            }

            let mut type_candidates = Vec::new();
            let mut lifetime_candidates = Vec::new();

            for item in &file.items {
                if let Item::Struct(item_struct) = item {
                    type_candidates.push(item_struct.ident.to_string());
                }
                if let Item::Enum(item_enum) = item {
                    type_candidates.push(item_enum.ident.to_string());
                }
                if let Item::Type(item_type) = item {
                    type_candidates.push(item_type.ident.to_string());
                }
                if let Item::Trait(item_trait) = item {
                    type_candidates.push(item_trait.ident.to_string());
                }
                if let Item::Impl(item_impl) = item {
                    if let Some((_, path, _)) = &item_impl.trait_ {
                        if let Some(segment) = path.segments.last() {
                            type_candidates.push(segment.ident.to_string());
                        }
                    }
                }
            }

            if type_candidates.is_empty() {
                type_candidates.push("i32".to_string());
                type_candidates.push("f32".to_string());
                type_candidates.push("i64".to_string());
                type_candidates.push("f64".to_string());
            }

            for item in &file.items {
                if let Item::Struct(item_struct) = item {
                    for param in &item_struct.generics.params {
                        if let GenericParam::Type(type_param) = param {
                            type_candidates.push(type_param.ident.to_string());
                        }
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            lifetime_candidates.push(lifetime_param.lifetime.ident.to_string());
                        }
                    }
                }
                if let Item::Enum(item_enum) = item {
                    for param in &item_enum.generics.params {
                        if let GenericParam::Type(type_param) = param {
                            type_candidates.push(type_param.ident.to_string());
                        }
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            lifetime_candidates.push(lifetime_param.lifetime.ident.to_string());
                        }
                    }
                }
                if let Item::Fn(item_fn) = item {
                    for param in &item_fn.sig.generics.params {
                        if let GenericParam::Type(type_param) = param {
                            type_candidates.push(type_param.ident.to_string());
                        }
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            lifetime_candidates.push(lifetime_param.lifetime.ident.to_string());
                        }
                    }
                }
                if let Item::Impl(item_impl) = item {
                    for param in &item_impl.generics.params {
                        if let GenericParam::Type(type_param) = param {
                            type_candidates.push(type_param.ident.to_string());
                        }
                        if let GenericParam::Lifetime(lifetime_param) = param {
                            lifetime_candidates.push(lifetime_param.lifetime.ident.to_string());
                        }
                    }
                }
            }

            let mut rng = thread_rng();
            type_candidates.shuffle(&mut rng);
            lifetime_candidates.shuffle(&mut rng);

            let mut new_generic_arguments = Vec::new();

            for _ in 0..num_generics {
                if let Some(candidate) = type_candidates.first() {
                    new_generic_arguments.push(candidate.clone());
                } else {
                    new_generic_arguments.push("i32".to_string());
                }
            }

            if let Some(candidate) = type_candidates.first() {
                new_generic_arguments.push(candidate.clone());
            } else {
                new_generic_arguments.push("i32".to_string());
            }

            struct StructInstantiationReplacer {
                target_line: usize,
                new_generic_arguments: Vec<String>,
            }

            impl VisitMut for StructInstantiationReplacer {
                fn visit_expr_mut(&mut self, node: &mut Expr) {
                    if let Expr::Struct(expr_struct) = node {
                        if expr_struct.span().start().line == self.target_line {
                            if let Some(path_segment) = expr_struct.path.segments.last_mut() {
                                if let PathArguments::AngleBracketed(arguments) =
                                    &mut path_segment.arguments
                                {
                                    let mut new_args = Punctuated::new();
                                    for arg in &self.new_generic_arguments {
                                        if arg.starts_with('\'') {
                                            new_args.push(GenericArgument::Lifetime(
                                                Lifetime::new(arg, Span::call_site()),
                                            ));
                                        } else {
                                            new_args.push(GenericArgument::Type(Type::Path(
                                                TypePath {
                                                    qself: None,
                                                    path: SynPath::from(Ident::new(
                                                        arg,
                                                        Span::call_site(),
                                                    )),
                                                },
                                            )));
                                        }
                                    }
                                    arguments.args = new_args;
                                }
                            }
                        }
                    }
                    syn::visit_mut::visit_expr_mut(self, node);
                }
            }

            let mut replacer = StructInstantiationReplacer {
                target_line: *target_line,
                new_generic_arguments,
            };
            replacer.visit_file_mut(file);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets struct instantiations with generic arguments by introducing an extra generic parameter. It first identifies all candidate struct instantiations, then randomly selects one to modify. The mutator ensures that the new generic argument is a valid type or lifetime available in the current scope, prioritizing existing types and lifetimes in the file. By increasing the number of generic arguments beyond what the struct definition expects, this transformation aims to stress-test the compiler's handling of generic parameter resolution and error reporting, potentially exposing bugs related to generic argument parsing and type checking."
    }
}