use proc_macro2::{Span, *};
use quote::*;
use rand::{seq::SliceRandom, thread_rng, Rng};
use std::{collections::HashSet, fs, ops::Range, panic, path::Path, process::Command, *};
use syn::{
    parse_quote, punctuated::Punctuated, spanned::Spanned, token, visit::Visit, visit_mut::VisitMut,
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
    PathArguments, ReturnType, Stmt, TraitBound, TraitBoundModifier, Type, TypeImplTrait,
    TypeParamBound, TypePath,
};

use crate::mutator::Mutator;

pub struct Alter_Array_Length_And_Type_In_Struct_17;

impl Mutator for Alter_Array_Length_And_Type_In_Struct_17 {
    fn name(&self) -> &str {
        "Alter_Array_Length_And_Type_In_Struct_17"
    }

    fn mutate(&self, file: &mut syn::File) {
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for field in &mut item_struct.fields {
                    if let Type::Array(type_array) = &mut field.ty {
                        if let syn::Expr::Lit(expr_lit) = &type_array.len {
                            if let syn::Lit::Int(lit_int) = &expr_lit.lit {
                                // Alter array length
                                let new_len: usize = lit_int.base10_parse().unwrap() * 2;
                                let new_len_expr: Expr = parse_quote! { #new_len };
                                type_array.len = new_len_expr;
                            }
                        }
                        // Alter array type by changing it to u32 for demonstration
                        if let Type::Path(TypePath { ref mut path, .. }) = *type_array.elem.clone() {
                            if path.segments.last().unwrap().ident == "i32" {
                                path.segments.last_mut().unwrap().ident = Ident::new("u32", Span::call_site());
                            }
                        }
                    }
                }
            }
        }
    }

    fn chain_of_thought(&self) -> &str {
        "This mutator changes the length of an array within a struct field by doubling it, potentially exposing compiler layout issues. It also converts i32 array element types to u32 to test type consistency and compiler behavior with varied types. This transformation aims to reveal compiler bugs related to memory layout and type handling by altering both the size and type of arrays in structs."
    }
}