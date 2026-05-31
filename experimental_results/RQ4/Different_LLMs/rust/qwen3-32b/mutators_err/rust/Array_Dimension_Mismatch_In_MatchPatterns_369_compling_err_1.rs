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

pub struct Array_Dimension_Mismatch_In_MatchPatterns_369;

impl Mutator for Array_Dimension_Mismatch_In_MatchPatterns_369 {
    fn name(&self) -> &str {
        "Array_Dimension_Mismatch_In_MatchPatterns_369"
    }
    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let syn::Item::Fn(func) = item {
                if func.sig.ident == "main" {
                    continue;
                }
                for input in &mut func.sig.inputs {
                    if let FnArg::Typed(pat_type) = input {
                        if let Type::Array(array) = &*pat_type.ty {
                            // Calculate current depth
                            let mut current_depth = 1;
                            let mut current_elem = &array.elem;
                            while let Type::Array(inner) = current_elem {
                                current_depth += 1;
                                current_elem = &inner.elem;
                            }
                            // Decide target depth
                            let target_depth = if current_depth > 1 {
                                current_depth - 1
                            } else {
                                current_depth + 1
                            };
                            // Modify the type
                            if target_depth < current_depth {
                                // Decrease depth: take inner array
                                let mut new_type = array.elem.clone();
                                let mut depth_to_go = target_depth;
                                while depth_to_go > 1 {
                                    if let Type::Array(inner) = &*new_type {
                                        new_type = inner.elem.clone();
                                        depth_to_go -= 1;
                                    } else {
                                        break;
                                    }
                                }
                                pat_type.ty = Box::new(new_type);
                            } else {
                                // Increase depth: wrap current type in a new array with new const
                                let new_const_name = format!("N{}", thread_rng().gen_range(0..100));
                                let new_const = parse_quote!(const #new_const_name: usize);
                                func.sig.generics.params.push(new_const);
                                let new_array = parse_quote!([#array; #new_const_name]);
                                pat_type.ty = Box::new(new_array);
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator alters the array dimensionality of function parameters with generic const parameters. It either reduces or increases the nesting level of the array type, creating a mismatch between the actual array structure and the match patterns in the function body. This transformation exploits the compiler's handling of const generics and pattern matching, potentially leading to internal compiler errors (ICE) when type inference encounters inconsistent array structures."
    }
}