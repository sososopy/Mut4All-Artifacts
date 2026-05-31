use proc_macro2::{Span, TokenStream};
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

pub struct Replace_Macro_In_Async_With_Trailing_Semicolon_198;

impl Mutator for Replace_Macro_In_Async_With_Trailing_Semicolon_198 {
    fn name(&self) -> &str {
        "Replace_Macro_In_Async_With_Trailing_Semicolon_198"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut async_macro_calls = Vec::new();
        let mut existing_trailing_semicolon_macros = HashSet::new();
        
        // First pass: collect all macro calls in async contexts and existing macros that produce trailing semicolons
        struct AsyncMacroFinder {
            calls: Vec<(ExprMacro, Vec<Attribute>)>,
            existing_macros: HashSet<Ident>,
        }
        impl Visit<'_> for AsyncMacroFinder {
            fn visit_expr(&mut self, expr: &Expr) {
                if let Expr::Async(expr_async) = expr {
                    // Collect lint attributes from the async block
                    let lint_attrs = expr_async.attrs.clone();
                    // Visit inside async block to find macro calls
                    syn::visit::visit_block(self, &expr_async.block);
                    // Store macro calls with their associated lint attributes
                    for call in &self.calls {
                        if call.1.is_empty() {
                            self.calls.push((call.0.clone(), lint_attrs.clone()));
                        }
                    }
                }
                if let Expr::Macro(expr_macro) = expr {
                    // Check if this macro expands to something with trailing semicolon
                    // Simple heuristic: look for macros that end with semicolon in their expansion pattern
                    if let Some(path_segment) = expr_macro.mac.path.segments.last() {
                        self.existing_macros.insert(path_segment.ident.clone());
                    }
                    self.calls.push((expr_macro.clone(), Vec::new()));
                }
                syn::visit::visit_expr(self, expr);
            }
            fn visit_item_macro(&mut self, item: &ItemMacro) {
                // Record macro definitions that could produce trailing semicolons
                if let Some(path_segment) = item.mac.path.segments.last() {
                    self.existing_macros.insert(path_segment.ident.clone());
                }
                syn::visit::visit_item_macro(self, item);
            }
        }
        
        let mut finder = AsyncMacroFinder {
            calls: Vec::new(),
            existing_macros: HashSet::new(),
        };
        finder.visit_file(&file);
        
        // Determine if we need to add a new macro definition
        let needs_new_macro = finder.existing_macros.is_empty();
        
        // Second pass: mutate the collected macro calls
        struct AsyncMacroMutator<'a> {
            calls: &'a Vec<(ExprMacro, Vec<Attribute>)>,
            existing_macros: &'a HashSet<Ident>,
            needs_new_macro: bool,
        }
        impl<'a> VisitMut for AsyncMacroMutator<'a> {
            fn visit_expr_mut(&mut self, expr: &mut Expr) {
                if let Expr::Macro(expr_macro) = expr {
                    // Check if this macro call is one we collected
                    for (original_macro, lint_attrs) in self.calls {
                        if expr_macro.mac.path == original_macro.mac.path && expr_macro.mac.tokens.to_string() == original_macro.mac.tokens.to_string() {
                            // Replace with a trailing semicolon macro
                            let replacement_macro_ident = if self.needs_new_macro {
                                Ident::new("mutated_macro", Span::call_site())
                            } else {
                                // Pick a random existing macro
                                let mut rng = thread_rng();
                                let macros_vec: Vec<&Ident> = self.existing_macros.iter().collect();
                                if let Some(random_macro) = macros_vec.choose(&mut rng) {
                                    random_macro.clone()
                                } else {
                                    Ident::new("mutated_macro", Span::call_site())
                                }
                            };
                            
                            // Create new macro call with same arguments
                            let new_macro = ExprMacro {
                                attrs: lint_attrs.clone(),
                                mac: Macro {
                                    path: SynPath {
                                        leading_colon: None,
                                        segments: Punctuated::from_iter(vec![PathSegment {
                                            ident: replacement_macro_ident,
                                            arguments: PathArguments::None,
                                        }]),
                                    },
                                    bang_token: token::Bang::default(),
                                    delimiter: original_macro.mac.delimiter.clone(),
                                    tokens: original_macro.mac.tokens.clone(),
                                },
                            };
                            *expr = Expr::Macro(new_macro);
                            break;
                        }
                    }
                }
                syn::visit_mut::visit_expr_mut(self, expr);
            }
        }
        
        let mut mutator = AsyncMacroMutator {
            calls: &finder.calls,
            existing_macros: &finder.existing_macros,
            needs_new_macro,
        };
        mutator.visit_file_mut(file);
        
        // If needed, add the new macro definition to the file
        if needs_new_macro {
            let new_macro_item = Item::Macro(ItemMacro {
                attrs: Vec::new(),
                ident: Some(Ident::new("mutated_macro", Span::call_site())),
                mac: Macro {
                    path: SynPath {
                        leading_colon: None,
                        segments: Punctuated::from_iter(vec![PathSegment {
                            ident: Ident::new("mutated_macro", Span::call_site()),
                            arguments: PathArguments::None,
                        }]),
                    },
                    bang_token: token::Bang::default(),
                    delimiter: MacroDelimiter::Paren(Paren::default()),
                    tokens: TokenStream::from(quote::quote! { () => { true; } }),
                },
                semi_token: Some(token::Semi::default()),
            });
            file.items.push(new_macro_item);
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets macro invocations within async blocks or functions that have lint attributes applied. It replaces such macro calls with another macro that generates trailing semicolons in its expansion. If no suitable macro exists in the program, it introduces a new macro definition that expands to 'true;'. This mutation tests the compiler's handling of macro expansions with trailing semicolons within async contexts, particularly focusing on interactions with lint attributes like #[expect], #[allow], or #[warn]. The goal is to provoke ICEs related to macro expansion and lint attribute processing in async code."
    }
}