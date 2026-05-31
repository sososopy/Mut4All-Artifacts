use syn::parse_quote;
use crate::mutator::Mutator;
use syn::Item;
use syn::ItemConst;
use syn::Ident;
use syn::ExprMatch;
use syn::Pat;
use syn::visit_mut::VisitMut;
use std::collections::HashMap;

pub struct Replace_Const_Type_In_Match_71;

impl Mutator for Replace_Const_Type_In_Match_71 {
    fn name(&self) -> &str {
        "Replace_Const_Type_In_Match_71"
    }
    fn mutate(&self, file: &mut syn::File) {
        let mut constants_map = std::collections::HashMap::new();
        for item in &mut file.items {
            if let Item::Const(item_const) = item {
                constants_map.insert(&item_const.ident, item_const);
            }
        }

        struct MatchArmVisitor<'a> {
            constants: std::collections::HashMap<&'a Ident, &'a mut ItemConst>,
            constants_to_modify: Vec<&'a mut ItemConst>,
        }

        impl<'a> MatchArmVisitor<'a> {
            fn new(constants: std::collections::HashMap<&'a Ident, &'a mut ItemConst>) -> Self {
                Self {
                    constants,
                    constants_to_modify: Vec::new(),
                }
            }
        }

        impl<'a> VisitMut for MatchArmVisitor<'a> {
            fn visit_expr_match_mut(&mut self, expr: &mut ExprMatch) {
                for arm in &mut expr.arms {
                    if let Pat::Path(pat_path) = &arm.pat {
                        if let Some(ident) = pat_path.path.get_ident() {
                            if let Some(constant) = self.constants.get(&ident) {
                                if !self.constants_to_modify.contains(&constant) {
                                    self.constants_to_modify.push(constant);
                                }
                            }
                        }
                    }
                }
            }
        }

        let mut visitor = MatchArmVisitor::new(constants_map);
        visitor.visit_file_mut(file);

        for constant in visitor.constants_to_modify {
            constant.ty = parse_quote!(i32);
            if let Some(expr) = constant.expr.as_mut() {
                *expr = parse_quote!(0);
            }
        }
    }
    fn chain_of_thought(&self) -> &str {
        "The mutation operator identifies constants used in match arm patterns and replaces their type with an incompatible one (e.g., from unit type `()` to `i32`). This creates a type mismatch between the constant's definition and the match expression's expected type, testing the compiler's ability to handle invalid pattern matching scenarios and type coherence errors."
    }
}