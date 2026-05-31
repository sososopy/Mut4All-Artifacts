use proc_macro2::{Span, *};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command, *};
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

pub struct Modify_Macro_Invocation_With_Expression_Inside_Path_206;

impl Mutator for Modify_Macro_Invocation_With_Expression_Inside_Path_206 {
    fn name(&self) -> &str {
        "Modify_Macro_Invocation_With_Expression_Inside_Path_206"
    }
    fn mutate(&self, file: &mut syn::File) {
        struct MacroPathExprReplacer {
            replacements: Vec<(SynPath, SynPath)>,
        }

        impl MacroPathExprReplacer {
            fn find_replacements(&mut self, file: &syn::File) {
                let mut visitor = MacroPathExprFinder {
                    replacements: &mut self.replacements,
                };
                visitor.visit_file(file);
            }
        }

        impl VisitMut for MacroPathExprReplacer {
            fn visit_path_mut(&mut self, node: &mut SynPath) {
                for (target, replacement) in &self.replacements {
                    if node == target {
                        *node = replacement.clone();
                        break;
                    }
                }
                syn::visit_mut::visit_path_mut(self, node);
            }
        }

        struct MacroPathExprFinder<'a> {
            replacements: &'a mut Vec<(SynPath, SynPath)>,
        }

        impl<'a> Visit<'a> for MacroPathExprFinder<'a> {
            fn visit_macro(&mut self, node: &'a syn::Macro) {
                let mut found_paths = Vec::new();
                for token_tree in node.tokens.clone() {
                    if let proc_macro2::TokenTree::Group(group) = token_tree {
                        let group_stream = group.stream();
                        let mut inner_stream = group_stream.clone().into_iter();
                        while let Some(inner_token_tree) = inner_stream.next() {
                            if let proc_macro2::TokenTree::Ident(ident) = inner_token_tree {
                                if ident == "path" {
                                    found_paths.push(group_stream.clone());
                                }
                            }
                        }
                    }
                }
                for path_stream in found_paths {
                    let path: SynPath = syn::parse2(path_stream.clone()).unwrap();
                    let mut has_expr_inside = false;
                    for segment in &path.segments {
                        if let PathArguments::AngleBracketed(args) = &segment.arguments {
                            for arg in &args.args {
                                if matches!(arg, GenericArgument::Const(_)) {
                                    has_expr_inside = true;
                                    break;
                                }
                            }
                        }
                        if has_expr_inside {
                            break;
                        }
                    }
                    if has_expr_inside {
                        let replacement_path: SynPath = parse_quote!(inline::SomeValidPath);
                        self.replacements.push((path, replacement_path));
                    }
                }
                syn::visit::visit_macro(self, node);
            }
        }

        let mut replacer = MacroPathExprReplacer {
            replacements: Vec::new(),
        };
        replacer.find_replacements(file);
        replacer.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "This mutator targets macro invocations where a path pattern includes an embedded expression or block. It replaces such paths with a valid, simpler path, like `inline::SomeValidPath`, to avoid ambiguous or complex scenarios in macro resolution. This transformation tests the compiler's ability to handle path-based macro patterns and ensures robustness against intricate path expressions."
    }
}