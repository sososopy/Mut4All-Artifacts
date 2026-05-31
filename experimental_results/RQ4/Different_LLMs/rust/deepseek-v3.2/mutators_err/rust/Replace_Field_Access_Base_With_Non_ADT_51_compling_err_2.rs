use proc_macro2::{Span,*};
use quote::*;
use rand::{Rng, seq::SliceRandom, thread_rng};
use regex::Regex;
use std::{collections::HashSet, default, fs, ops::Range, panic, path::Path, process::Command,*};
use syn::{
    BoundLifetimes, Expr, ExprCall, ExprPath, File, FnArg, GenericArgument, GenericParam, Ident,
    Item, ItemFn, ItemStruct, ItemType, Lifetime, LifetimeParam, Local, Pat, PatType, Path as SynPath,
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

pub struct Replace_Field_Access_Base_With_Non_ADT_51;

impl Mutator for Replace_Field_Access_Base_With_Non_ADT_51 {
    fn name(&self) -> &str {
        "Replace_Field_Access_Base_With_Non_ADT_51"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut visitor = ReplaceFieldAccessBaseVisitor::new();
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets field access expressions `<expr>.<ident>`. It replaces the base expression `<expr>` with a non-ADT type expression (array, slice, primitive, tuple) while keeping the field identifier unchanged. This transformation aims to trigger internal compiler errors when the compiler incorrectly assumes the base expression has an ADT type during certain passes, testing the compiler's handling of invalid field accesses on non-struct/enum/union types."
    }
}

struct ReplaceFieldAccessBaseVisitor {
    available_types: Vec<syn::Type>,
}

impl ReplaceFieldAccessBaseVisitor {
    fn new() -> Self {
        Self {
            available_types: Vec::new(),
        }
    }

    fn collect_types(&mut self, file: &syn::File) {
        for item in &file.items {
            match item {
                syn::Item::Struct(item_struct) => {
                    self.available_types.push(syn::Type::Path(syn::TypePath {
                        qself: None,
                        path: item_struct.ident.clone().into(),
                    }));
                }
                syn::Item::Enum(item_enum) => {
                    self.available_types.push(syn::Type::Path(syn::TypePath {
                        qself: None,
                        path: item_enum.ident.clone().into(),
                    }));
                }
                syn::Item::Type(item_type) => {
                    self.available_types.push(*item_type.ty.clone());
                }
                _ => {}
            }
        }
    }

    fn find_primitive_type(&self) -> syn::Type {
        if self.available_types.is_empty() {
            syn::Type::Path(syn::TypePath {
                qself: None,
                path: syn::Path::from(syn::Ident::new("i32", Span::call_site())),
            })
        } else {
            self.available_types[0].clone()
        }
    }

    fn create_non_adt_base(&self) -> syn::Expr {
        let primitive_type = self.find_primitive_type();
        let default_value = match &primitive_type {
            syn::Type::Path(type_path) if type_path.path.is_ident("i32") => {
                syn::Expr::Lit(syn::ExprLit {
                    attrs: Vec::new(),
                    lit: syn::Lit::Int(syn::LitInt::new("0", Span::call_site())),
                })
            }
            syn::Type::Path(type_path) if type_path.path.is_ident("bool") => {
                syn::Expr::Lit(syn::ExprLit {
                    attrs: Vec::new(),
                    lit: syn::Lit::Bool(syn::LitBool::new(false, Span::call_site())),
                })
            }
            syn::Type::Path(type_path) if type_path.path.is_ident("char") => {
                syn::Expr::Lit(syn::ExprLit {
                    attrs: Vec::new(),
                   1: syn::Lit::Char(syn::LitChar::new('\0', Span::call_site())),
                })
            }
            syn::Type::Tuple(_) => {
                syn::Expr::Lit(syn::ExprLit {
                    attrs: Vec::new(),
                    lit: syn::Lit::Verbatim(syn::LitStr::new("()", Span::call_site()).into()),
                })
            }
            _ => {
                syn::Expr::Lit(syn::ExprLit {
                    attrs: Vec::new(),
                    lit: syn::Lit::Verbatim(syn::LitStr::new("()", Span::call_site()).into()),
                })
            }
        };

        syn::Expr::Array(syn::ExprArray {
            attrs: Vec::new(),
            bracket_token: syn::token::Bracket::default(),
            elems: Punctuated::from_iter(vec![default_value]),
        })
    }
}

impl VisitMut for ReplaceFieldAccessBaseVisitor {
    fn visit_expr_mut(&mut self, expr: &mut syn::Expr) {
        if let syn::Expr::Field(field_expr) = expr {
            let new_base = self.create_non_adt_base();
            field_expr.base = Box::new(new_base);
        }
        syn::visit_mut::visit_expr_mut(self, expr);
    }
}