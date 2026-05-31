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

pub struct Introduce_Undeclared_Lifetime_In_Pattern_241;

impl Mutator for Introduce_Undeclared_Lifetime_In_Pattern_241 {
    fn name(&self) -> &str {
        "Introduce_Undeclared_Lifetime_In_Pattern_241"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut rng = thread_rng();
        let mut visitor = PatternVisitor {
            rng: &mut rng,
            modified: false,
        };
        visitor.visit_file_mut(file);
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator targets pattern-matching expressions (if let, match) that use an enum variant containing a reference type but lacking lifetime parameters. It modifies the pattern to include a reference literal (like a string slice) or a reference to an existing variable, creating a mismatch between the pattern's constructor and the expected constructor for &T. This can trigger internal compiler errors in pattern compatibility checking when the enum variant's field is a reference type &T without explicit lifetime parameters."
    }
}

struct PatternVisitor<'a, R: Rng> {
    rng: &'a mut R,
    modified: bool,
}

impl<'a, R: Rng> VisitMut for PatternVisitor<'a, R> {
    fn visit_expr_match_mut(&mut self, expr: &mut ExprMatch) {
        self.visit_expr_mut(&mut expr.expr);
        for arm in &mut expr.arms {
            self.visit_pat_mut(&mut arm.pat);
            if let Some(guard) = &mut arm.guard {
                self.visit_expr_mut(&mut *guard.1);
            }
            self.visit_expr_mut(&mut arm.body);
        }
    }

    fn visit_expr_let_mut(&mut self, expr: &mut ExprLet) {
        if let Some((_, else_branch)) = &mut expr.else_branch {
            self.visit_expr_mut(else_branch);
        }
        self.visit_pat_mut(&mut expr.pat);
        self.visit_expr_mut(&mut expr.expr);
        self.visit_block_mut(&mut expr.then_branch);
    }

    fn visit_pat_mut(&mut self, pat: &mut Pat) {
        if let Pat::TupleStruct(pat_tuple_struct) = pat {
            if let Some(qself) = &pat_tuple_struct.qself {
                if let Type::Path(type_path) = &*qself.ty {
                    if let Some(segment) = type_path.path.segments.last() {
                        if let PathArguments::AngleBracketed(args) = &segment.arguments {
                            if args.args.is_empty() {
                                return;
                            }
                        }
                    }
                }
            }
            let path = &pat_tuple_struct.path;
            if let Some(segment) = path.segments.last() {
                let variant_name = segment.ident.to_string();
                let mut collector = EnumCollector {
                    enum_name: None,
                    variant_name: &variant_name,
                    found: false,
                    has_lifetime_param: false,
                    has_reference_field: false,
                };
                let mut file = File {
                    shebang: None,
                    attrs: vec![],
                    items: vec![],
                };
                collector.visit_file_mut(&mut file);
                if collector.found && collector.has_reference_field && !collector.has_lifetime_param {
                    if !self.modified && self.rng.gen_bool(0.5) {
                        let choices = ["\"mutation\"", "&42", "&true", "&'x'"];
                        let chosen = choices.choose(self.rng).unwrap();
                        let new_pat_str = format!("{}::{} ({})", path.segments.iter().map(|s| s.ident.to_string()).collect::<Vec<_>>().join("::"), variant_name, chosen);
                        let new_pat: Pat = syn::parse_str(&new_pat_str).unwrap_or_else(|_| syn::parse_str(&variant_name).unwrap());
                        *pat = new_pat;
                        self.modified = true;
                    }
                }
            }
        }
        syn::visit_mut::visit_pat_mut(self, pat);
    }
}

struct EnumCollector<'a> {
    enum_name: Option<String>,
    variant_name: &'a str,
    found: bool,
    has_lifetime_param: bool,
    has_reference_field: bool,
}

impl<'a> VisitMut for EnumCollector<'a> {
    fn visit_item_enum_mut(&mut self, item: &mut ItemEnum) {
        let old_enum_name = self.enum_name.take();
        self.enum_name = Some(item.ident.to_string());
        for param in &item.generics.params {
            if let GenericParam::Lifetime(_) = param {
                self.has_lifetime_param = true;
            }
        }
        for variant in &mut item.variants {
            if variant.ident.to_string() == *self.variant_name {
                self.found = true;
                for field in &variant.fields {
                    if let Type::Reference(_) = field.ty {
                        self.has_reference_field = true;
                    }
                }
            }
        }
        syn::visit_mut::visit_item_enum_mut(self, item);
        self.enum_name = old_enum_name;
    }
}