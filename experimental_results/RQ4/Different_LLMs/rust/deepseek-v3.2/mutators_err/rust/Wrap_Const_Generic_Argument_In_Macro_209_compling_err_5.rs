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

pub struct Wrap_Const_Generic_Argument_In_Macro_209;

impl Mutator for Wrap_Const_Generic_Argument_In_Macro_209 {
    fn name(&self) -> &str {
        "Wrap_Const_Generic_Argument_In_Macro_209"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut macro_defined = false;
        let macro_name = Ident::new("wrap_const", Span::call_site());
        
        let mut visitor = ConstGenericVisitor {
            macro_name: &macro_name,
            rng: &mut rng,
            macro_defined: &mut macro_defined,
        };
        visitor.visit_file_mut(file);
        
        if *visitor.macro_defined {
            let macro_def = parse_quote! {
                macro_rules! wrap_const {
                    ($v:expr) => { $v };
                }
            };
            file.items.insert(0, Item::Macro(syn::ItemMacro {
                attrs: Vec::new(),
                ident: Some(macro_name.clone()),
                mac: syn::Macro {
                    path: syn::Path::from(macro_name.clone()),
                    bang_token: syn::token::Bang::default(),
                    delimiter: syn::MacroDelimiter::Brace(token::Brace::default()),
                    tokens: proc_macro2::TokenStream::new(),
                },
                semi_token: None,
            }));
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets const generic arguments that are simple identifiers (const parameter names) in struct definitions, enum definitions, or function signatures. It wraps these identifiers in a macro invocation (wrap_const!) that expands to the same identifier. If the macro is not already defined, it adds a macro_rules definition at the crate root. This transformation forces the compiler to resolve const generic arguments through macro expansion, potentially triggering internal compiler errors in constant evaluation or HIR mapping. The mutation avoids complex expressions to prevent nested complexities."
    }
}

struct ConstGenericVisitor<'a, R: Rng> {
    macro_name: &'a Ident,
    rng: &'a mut R,
    macro_defined: &'a mut bool,
}

impl<'a, R: Rng> VisitMut for ConstGenericVisitor<'a, R> {
    fn visit_type_mut(&mut self, ty: &mut Type) {
        if let Type::Path(TypePath { path, qself: None }) = ty {
            for segment in &mut path.segments {
                if let PathArguments::AngleBracketed(args) = &mut segment.arguments {
                    for arg in &mut args.args {
                        if let GenericArgument::Const(expr) = arg {
                            if let Expr::Path(ExprPath { path: expr_path, attrs: _, qself: None }) = expr {
                                if expr_path.segments.len() == 1 {
                                    let ident = &expr_path.segments[0].ident;
                                    if self.rng.gen_bool(0.5) {
                                        *self.macro_defined = true;
                                        let new_expr: Expr = parse_quote! {
                                            wrap_const!(#ident)
                                        };
                                        *expr = new_expr;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        syn::visit_mut::visit_type_mut(self, ty);
    }
    
    fn visit_item_mut(&mut self, item: &mut Item) {
        if let Item::Macro(item_macro) = item {
            if let Some(path_seg) = item_macro.mac.path.segments.last() {
                if path_seg.ident == "wrap_const" {
                    *self.macro_defined = true;
                }
            }
        }
        syn::visit_mut::visit_item_mut(self, item);
    }
}