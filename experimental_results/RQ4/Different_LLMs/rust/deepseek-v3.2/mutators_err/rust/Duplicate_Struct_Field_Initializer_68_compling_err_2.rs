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

pub struct Duplicate_Struct_Field_Initializer_68;

impl Mutator for Duplicate_Struct_Field_Initializer_68 {
    fn name(&self) -> &str {
        "Duplicate_Struct_Field_Initializer_68"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut struct_defs = Vec::new();
        for item in &file.items {
            if let Item::Struct(struct_def) = item {
                if struct_def.fields.len() >= 2 {
                    struct_defs.push(struct_def.clone());
                }
            }
        }
        for item in &mut file.items {
            if let Item::Const(const_item) = item {
                if let Expr::Struct(struct_expr) = &*const_item.expr {
                    for struct_def in &struct_defs {
                        if struct_expr.path.segments.last().unwrap().ident == struct_def.ident {
                            let mut new_fields = struct_expr.fields.clone();
                            let mut rng = thread_rng();
                            let field_names: Vec<Ident> = struct_def.fields.iter().filter_map(|f| f.ident.clone()).collect();
                            if field_names.len() >= 2 {
                                let target_field = field_names.choose(&mut rng).unwrap();
                                let duplicate_expr = find_compatible_expr(&struct_expr.fields, &field_names, &struct_def.fields);
                                let new_field = FieldValue {
                                    attrs: Vec::new(),
                                    member: Member::Named(target_field.clone()),
                                    colon_token: Some(token::Colon::default()),
                                    expr: *duplicate_expr,
                                };
                                new_fields.push(new_field);
                                let new_struct_expr = ExprStruct {
                                    attrs: struct_expr.attrs.clone(),
                                    qself: struct_expr.qself.clone(),
                                    path: struct_expr.path.clone(),
                                    brace_token: struct_expr.brace_token.clone(),
                                    fields: new_fields,
                                    dot2_token: struct_expr.dot2_token,
                                    rest: struct_expr.rest.clone(),
                                };
                                const_item.expr = Box::new(Expr::Struct(new_struct_expr));
                            }
                        }
                    }
                }
            }
            if let Item::Static(static_item) = item {
                if let Expr::Struct(struct_expr) = &*static_item.expr {
                    for struct_def in &struct_defs {
                        if struct_expr.path.segments.last().unwrap().ident == struct_def.ident {
                            let mut new_fields = struct_expr.fields.clone();
                            let mut rng = thread_rng();
                            let field_names: Vec<Ident> = struct_def.fields.iter().filter_map(|f| f.ident.clone()).collect();
                            if field_names.len() >= 2 {
                                let target_field = field_names.choose(&mut rng).unwrap();
                                let duplicate_expr = find_compatible_expr(&struct_expr.fields, &field_names, &struct_def.fields);
                                let new_field = FieldValue {
                                    attrs: Vec::new(),
                                    member: Member::Named(target_field.clone()),
                                    colon_token: Some(token::Colon::default()),
                                    expr: *duplicate_expr,
                                };
                                new_fields.push(new_field);
                                let new_struct_expr = ExprStruct {
                                    attrs: struct_expr.attrs.clone(),
                                    qself: struct_expr.qself.clone(),
                                    path: struct_expr.path.clone(),
                                    brace_token: struct_expr.brace_token.clone(),
                                    fields: new_fields,
                                    dot2_token: struct_expr.dot2_token,
                                    rest: struct_expr.rest.clone(),
                                };
                                static_item.expr = Box::new(Expr::Struct(new_struct_expr));
                            }
                        }
                    }
                }
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets struct initializations within const and static items. It duplicates a field name from the struct definition, adding a second initialization with the same name, shadowing the first. This mimics bugs where duplicate field names in static/const contexts trigger internal compiler errors during const-eval or type checking. It ensures type compatibility by using existing expressions or defaults."
    }
}

fn find_compatible_expr(fields: &Punctuated<FieldValue, Comma>, field_names: &Vec<Ident>, struct_fields: &Fields) -> Box<Expr> {
    let mut rng = thread_rng();
    for field in fields {
        if let Member::Named(ident) = &field.member {
            if field_names.contains(ident) {
                return Box::new(field.expr.clone());
            }
        }
    }
    for struct_field in struct_fields {
        if let Some(ident) = &struct_field.ident {
            if let Type::Path(type_path) = &struct_field.ty {
                let path = &type_path.path;
                if path.segments.last().unwrap().ident == "i32" {
                    return Box::new(Expr::Lit(ExprLit {
                        attrs: Vec::new(),
                        lit: Lit::Int(LitInt::new("0", Span::call_site())),
                    }));
                } else if path.segments.last().unwrap().ident == "bool" {
                    return Box::new(Expr::Lit(ExprLit {
                        attrs: Vec::new(),
                        lit: Lit::Bool(LitBool::new(false, Span::call_site())),
                    }));
                } else if path.segments.last().unwrap().ident == "char" {
                    return Box::new(Expr::Lit(ExprLit {
                        attrs: Vec::new(),
                        lit: Lit::Char(LitChar::new('\0', Span::call_site())),
                    }));
                } else if path.segments.last().unwrap().ident == "usize" {
                    return Box::new(Expr::Lit(ExprLit {
                        attrs: Vec::new(),
                        lit: Lit::Int(LitInt::new("0", Span::call_site())),
                    }));
                }
            }
        }
    }
    return Box::new(Expr::Call(ExprCall {
        attrs: Vec::new(),
        func: Box::new(Expr::Path(ExprPath {
            attrs: Vec::new(),
            qself: None,
            path: SynPath::from(Ident::new("Default", Span::call_site())),
        })),
        paren_token: Paren::default(),
        args: Punctuated::new(),
    }));
}