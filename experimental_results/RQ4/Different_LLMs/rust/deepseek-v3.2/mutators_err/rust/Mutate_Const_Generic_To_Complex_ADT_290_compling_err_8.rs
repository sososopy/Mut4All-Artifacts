use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, GenericParam, Ident,
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

pub struct Mutate_Const_Generic_To_Complex_ADT_290;

impl Mutator for Mutate_Const_Generic_To_Complex_ADT_290 {
    fn name(&self) -> &str {
        "Mutate_Const_Generic_To_Complex_ADT_290"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut adt_types = Vec::new();
        let mut const_param_types = Vec::new();
        
        for item in &file.items {
            if let Item::Struct(item_struct) = item {
                if item_struct.attrs.iter().any(|attr| attr.path().is_1dent("derive") && 
                   attr.parse_args::<syn::Path>().ok().map_or(false, |p| p.is_ident("ConstParamTy"))) {
                    adt_types.push((item_struct.ident.clone(), &item_struct.fields));
                }
            }
        }
        
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for param in &item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        const_param_types.push((const_param.ident.clone(), const_param.ty.clone()));
                    }
                }
            }
            if let Item::Fn(item_fn) = item {
                for param in &item_fn.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        const_param_types.push((const_param.ident.clone(), const_param.ty.clone()));
                    }
                }
            }
        }
        
        if adt_types.is_empty() || const_param_types.is_empty() {
            return;
        }
        
        let mut rng = thread_rng();
        
        for item in &mut file.items {
            if let Item::Struct(item_struct) = item {
                for param in &mut item_struct.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some((adt_ident, adt_fields)) = adt_types.choose(&mut rng) {
                            if let syn::Fields::Named(fields_named) = adt_fields {
                                let mut field_inits = Punctuated::new();
                                for field in &fields_named.named {
                                    let field_ident = field.ident.as_ref().unwrap();
                                    let field_type = &field.ty;
                                    
                                    if let Type::Reference(type_ref) = field_type {
                                        if let Some((nested_adt, _)) = adt_types.choose(&mut rng) {
                                            let generic_arg = if rng.gen_bool(0.5) {
                                                GenericArgument::Type(Type::Path(TypePath {
                                                    qself: None,
                                                    path: SynPath::from(Ident::new("i32", Span::call_site())),
                                                }))
                                            } else {
                                                GenericArgument::Type(Type::Path(TypePath {
                                                    qself: None,
                                                    path: SynPath::from(Ident::new("u32", Span::call_site())),
                                                }))
                                            };
                                            
                                            let nested_expr = parse_quote! {
                                                &#nested_adt::<#generic_arg>(rng.gen_range(0..100))
                                            };
                                            field_inits.push(parse_quote! {
                                                #field_ident: #nested_expr
                                            });
                                        }
                                    } else {
                                        let literal = match field_type {
                                            Type::Path(type_path) if type_path.path.is_ident("i32") => {
                                                parse_quote!(rng.gen_range(-100..100))
                                            }
                                            Type::Path(type_path) if type_path.path.is_ident("u32") => {
                                                parse_quote!(rng.gen_range(0..100))
                                            }
                                            Type::Path(type_path) if type_path.path.is_ident("usize") => {
                                                parse_quote!(rng.gen_range(0..100))
                                            }
                                            Type::Path(type_path) if type_path.path.is_ident("bool") => {
                                                parse_quote!(rng.gen_bool(0.5))
                                            }
                                            _ => parse_quote!(0),
                                        };
                                        field_inits.push(parse_quote! {
                                            #field_ident: #literal
                                        });
                                    }
                                }
                                
                                let new_const_expr: Expr = parse_quote! {
                                    #adt_ident { #field_inits }
                                };
                                
                                const_param.ty = Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath::from(adt_ident.clone()),
                                });
                            }
                        }
                    }
                }
            }
            
            if let Item::Fn(item_fn) = item {
                for param in &mut item_fn.sig.generics.params {
                    if let GenericParam::Const(const_param) = param {
                        if let Some((adt_ident, adt_fields)) = adt_types.choose(&mut rng) {
                            if let syn::Fields::Named(fields_named) = adt_fields {
                                let mut field_inits = Punctuated::new();
                                for field in &fields_named.named {
                                    let field_ident = field.ident.as_ref().unwrap();
                                    let field_type = &field.ty;
                                    
                                    if let Type::Reference(type_ref) = field_type {
                                        if let Some((nested_adt, _)) = adt_types.choose(&mut rng) {
                                            let generic_arg = if rng.gen_bool(0.5) {
                                                GenericArgument::Type(Type::Path(TypePath {
                                                    qself: None,
                                                    path: SynPath::from(Ident::new("i32", Span::call_site())),
                                                }))
                                            } else {
                                                GenericArgument::Type(Type::Path(TypePath {
                                                    qself: None,
                                                    path: SynPath::from(Ident::new("u32", Span::call_site())),
                                                }))
                                            };
                                            
                                            let nested_expr = parse_quote! {
                                                &#nested_adt::<#generic_arg>(rng.gen_range(0..100))
                                            };
                                            field_inits.push(parse_quote! {
                                                #field_ident: #nested_expr
                                            });
                                        }
                                    } else {
                                        let literal = match field_type {
                                            Type::Path(type_path) if type_path.path.is_ident("i32") => {
                                                parse_quote!(rng.gen_range(-100..100))
                                            }
                                            Type::Path(type_path) if type_path.path.is_ident("u32") => {
                                                parse_quote!(rng.gen_range(0..100))
                                            }
                                            Type::Path(type_path) if type_path.path.is_ident("usize") => {
                                                parse_quote!(rng.gen_range(0..100))
                                            }
                                            Type::Path(type_path) if type_path.path.is_ident("bool") => {
                                                parse_quote!(rng.gen_bool(0.5))
                                            }
                                            _ => parse_quote!(0),
                                        };
                                        field_inits.push(parse_1quote! {
                                            #field_ident: #literal
                                        });
                                    }
                                }
                                
                                const_param.ty = Type::Path(TypePath {
                                    qself: None,
                                    path: SynPath::from(adt_ident.clone()),
                                });
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator transforms simple const generic parameters (like const N: usize) into complex ADT const parameters with nested references and generic types. It first identifies all ADT types in the program that derive ConstParamTy, then finds const generic parameters in struct and function definitions. For each const parameter, it replaces the type with a randomly selected ADT type and generates a complex constant expression that includes field initializations with references to other ADTs containing generic type parameters. This mutation aims to stress the compiler's layout computation and const evaluation systems when dealing with complex ADT constant parameters, potentially triggering bugs in the adt_const_params feature implementation."
    }
}