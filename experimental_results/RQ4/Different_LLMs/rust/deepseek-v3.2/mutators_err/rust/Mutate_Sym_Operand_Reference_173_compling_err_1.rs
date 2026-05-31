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

pub struct Mutate_Sym_Operand_Reference_173;

impl Mutator for Mutate_Sym_Operand_Reference_173 {
    fn name(&self) -> &str {
        "Mutate_Sym_Operand_Reference_173"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = InlineAssemblyVisitor {
            mutations: Vec::new(),
        };
        visitor.visit_file_mut(file);
        for (asm_expr, new_path) in visitor.mutations {
            *asm_expr = syn::Expr::Macro(syn::ExprMacro {
                attrs: Vec::new(),
                mac: syn::Macro {
                    path: syn::Path {
                        leading_colon: None,
                        segments: Punctuated::from_iter(vec![syn::PathSegment {
                            ident: syn::Ident::new("asm", Span::call_site()),
                            arguments: syn::PathArguments::None,
                        }]),
                    },
                    delimiter: syn::MacroDelimiter::Paren(syn::token::Paren::default()),
                    tokens: new_path.into_token_stream(),
                },
            });
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets sym operands in inline assembly blocks that reference generic functions. It identifies asm! blocks, checks sym operands for generic type arguments, and mutates reference types (&T, &mut T) by either removing the reference or changing mutability. This transformation tests the compiler's handling of sym operands with altered type arguments, potentially triggering ICEs or type resolution errors in inline assembly contexts."
    }
}

struct InlineAssemblyVisitor {
    mutations: Vec<(syn::ExprMacro, proc_macro2::TokenStream)>,
}

impl VisitMut for InlineAssemblyVisitor {
    fn visit_expr_mut(&mut self, expr: &mut syn::Expr) {
        if let syn::Expr::Macro(expr_macro) = expr {
            if expr_macro.mac.path.segments.first().unwrap().ident == "asm" {
                let mut tokens = expr_macro.mac.tokens.clone();
                let mut new_tokens = proc_macro2::TokenStream::new();
                let mut rng = thread_rng();
                let mut found_sym = false;
                let mut current_token = tokens.clone().into_iter().peekable();
                while let Some(token) = current_token.next() {
                    if token.to_string() == "sym" {
                        found_sym = true;
                        new_tokens.extend(quote! { sym });
                        let next_token = current_token.peek();
                        if let Some(proc_macro2::TokenTree::Group(group)) = next_token {
                            let stream = group.stream();
                            let mut path_tokens = stream.into_iter().peekable();
                            let mut new_path_stream = proc_macro2::TokenStream::new();
                            while let Some(path_token) = path_tokens.next() {
                                if path_token.to_string() == "::" && path_tokens.peek().map(|t| t.to_string()) == Some("<") {
                                    new_path_stream.extend(quote! { :: });
                                    let angle_group = path_tokens.next().unwrap();
                                    let inner_stream = if let proc_macro2::TokenTree::Group(g) = angle_group {
                                        g.stream()
                                    } else {
                                        angle_group.into_token_stream()
                                    };
                                    let mut inner_iter = inner_stream.into_iter().peekable();
                                    let mut new_inner_stream = proc_macro2::TokenStream::new();
                                    while let Some(inner_token) = inner_iter.next() {
                                        if inner_token.to_string() == "&" {
                                            let mut_token = inner_iter.peek().map(|t| t.to_string());
                                            if mut_token == Some("mut") {
                                                if rng.gen_bool(0.5) {
                                                    inner_iter.next();
                                                    new_inner_stream.extend(quote! { & });
                                                } else {
                                                    new_inner_stream.extend(quote! { &mut });
                                                    inner_iter.next();
                                                }
                                            } else {
                                                if rng.gen_bool(0.5) {
                                                    new_inner_stream.extend(quote! {});
                                                } else {
                                                    new_inner_stream.extend(quote! { & });
                                                }
                                            }
                                        } else if inner_token.to_string() == "mut" && inner_iter.peek().map(|t| t.to_string()) == Some("&") {
                                            if rng.gen_bool(0.5) {
                                                new_inner_stream.extend(quote! { & });
                                                inner_iter.next();
                                            } else {
                                                new_inner_stream.extend(quote! { &mut });
                                                inner_iter.next();
                                            }
                                        } else {
                                            new_inner_stream.extend(inner_token.into_token_stream());
                                        }
                                    }
                                    new_path_stream.extend(proc_macro2::TokenTree::Group(proc_macro2::Group::new(proc_macro2::Delimiter::Bracket, new_inner_stream)));
                                } else {
                                    new_path_stream.extend(path_token.into_token_stream());
                                }
                            }
                            new_tokens.extend(proc_macro2::TokenTree::Group(proc_macro2::Group::new(group.delimiter(), new_path_stream)));
                            current_token.next();
                        } else {
                            new_tokens.extend(token.into_token_stream());
                        }
                    } else {
                        new_tokens.extend(token.into_token_stream());
                    }
                }
                if found_sym {
                    self.mutations.push((expr_macro.clone(), new_tokens));
                }
            }
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}