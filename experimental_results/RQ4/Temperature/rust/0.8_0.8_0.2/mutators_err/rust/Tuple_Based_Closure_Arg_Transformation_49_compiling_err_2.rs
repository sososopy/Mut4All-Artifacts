use proc_macro2::{Span, TokenStream};
use quote::quote;
use rand::{seq::SliceRandom, thread_rng, Rng};
use regex::Regex;
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit_mut::VisitMut, BoundLifetimes, Expr, ExprCall,
    ExprPath, File, FnArg, GenericArgument, GenericParam, Ident, Item, ItemFn, ItemStruct, Lifetime, LifetimeParam,
    Local, Pat, PatType, Path as SynPath, PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type,
    TypeImplTrait, TypeParamBound, TypePath, visit::Visit, token::Comma, token::{Paren, Plus},
};

use crate::mutator::Mutator;

pub struct Tuple_Based_Closure_Arg_Transformation_49;

impl Mutator for Tuple_Based_Closure_Arg_Transformation_49 {
    fn name(&self) -> &str {
        "Tuple_Based_Closure_Arg_Transformation_49"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(item_fn) = item {
                // Iterate over each function argument
                for input in &mut item_fn.sig.inputs {
                    if let syn::FnArg::Typed(pat_type) = input {
                        // Check if the type is a function type with multiple arguments
                        if let syn::Type::Path(type_path) = &*pat_type.ty {
                            if type_path.path.segments.len() == 1 {
                                let segment = &type_path.path.segments[0];
                                if segment.ident == "Fn" || segment.ident == "FnMut" || segment.ident == "FnOnce" {
                                    if let syn::PathArguments::AngleBracketed(angle_args) = &segment.arguments {
                                        if angle_args.args.len() > 1 {
                                            // Construct a tuple type for the arguments
                                            let tuple_type: syn::Type = parse_quote!((#angle_args));
                                            pat_type.ty = Box::new(Type::Path(TypePath {
                                                qself: None,
                                                path: path_with_tuple(segment.ident.clone(), tuple_type),
                                            }));
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                // Check function calls in the body
                let mut call_transformer = CallTransformer;
                call_transformer.visit_block_mut(&mut item_fn.block);
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutator transforms the closure arguments into a single tuple, modifying both the closure's signature and the corresponding function calls. It is designed to test Rust's ability to handle closure argument tuple transformation and uncover potential bugs in trait handling."
    }
}

fn path_with_tuple(ident: Ident, tuple_type: syn::Type) -> syn::Path {
    let mut segments = Punctuated::new();
    segments.push(PathSegment {
        ident,
        arguments: PathArguments::AngleBracketed(AngleBracketedGenericArguments {
            colon2_token: None,
            lt_token: token::Lt::default(),
            args: Punctuated::from_iter(vec![GenericArgument::Type(tuple_type)]),
            gt_token: token::Gt::default(),
        }),
    });
    syn::Path {
        leading_colon: None,
        segments,
    }
}

struct CallTransformer;

impl VisitMut for CallTransformer {
    fn visit_expr_call_mut(&mut self, node: &mut ExprCall) {
        // Modify function call arguments to be a single tuple
        if node.args.len() > 1 {
            let args: Expr = parse_quote!((#(#node.args),*));
            node.args.clear();
            node.args.push(args);
        }
        // Continue visiting nested expressions
        visit_mut::visit_expr_call_mut(self, node);
    }
}