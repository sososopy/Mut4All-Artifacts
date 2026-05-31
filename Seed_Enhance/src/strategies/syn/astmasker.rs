use proc_macro2::Span;
use quote::ToTokens;
use syn::{spanned::Spanned, visit_mut::VisitMut};

use crate::util::glob_range;

use super::strategy::consts::REMOVE_P;

// Note: The base code is copied from syn::visit_mut::VisitMut
// and modified automatically
pub(crate) struct ASTMasker {
    pub mask_data: Option<Span>,
    overlap_time: usize,
}
impl ASTMasker {
    pub fn new() -> Self {
        Self {
            mask_data: None,
            overlap_time: 1,
        }
    }
    fn do_work<N>(&mut self, nd: &N) -> Option<()>
    where
        N: ToTokens + Spanned,
    {
        // Some means return immediately,
        // None means continue traversing the AST

        // If the program is already masked, return None
        // Otherwise, randomly decide whether to mask or not
        // If we decide to mask, we store the code in self.mask_data
        // and return Some(())
        // If we decide not to mask, we return None

        let stream = nd.to_token_stream();
        let code = stream.to_string();

        let codelen_modifier = if code.len() <= 20 {
            1.
        } else {
            1. / (code.len() as f64 * 0.2)
        };
        let overlap_time_modifier = if self.overlap_time < 5 {
            1.
        } else {
            1. / (self.overlap_time * 2 + 1) as f64
        };
        let remove_p = REMOVE_P * overlap_time_modifier * codelen_modifier;

        if code.len() <= 0 {
            return None;
        }

        if glob_range(0. ..1.) > remove_p {
            return None;
        }

        self.mask_data = Some(nd.span());
        self.overlap_time += 1;
        Some(())
    }
    fn do_work_fallback<N>(&mut self, _: &N) -> Option<()> {
        // Some means return immediately,
        // None means continue traversing the AST

        // If the program is already masked, return None
        // Otherwise, randomly decide whether to mask or not
        // If we decide to mask, we store the code in self.mask_data
        // and return Some(())
        // If we decide not to mask, we return None
        match self.mask_data {
            Some(_) => Some(()),
            None => None,
        }
    }
}
impl VisitMut for ASTMasker {
    fn visit_abi_mut(&mut self, i: &mut syn::Abi) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_abi_mut(self, i);
            }
        }
    }

    fn visit_angle_bracketed_generic_arguments_mut(
        &mut self,
        i: &mut syn::AngleBracketedGenericArguments,
    ) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_angle_bracketed_generic_arguments_mut(self, i);
            }
        }
    }

    fn visit_arm_mut(&mut self, i: &mut syn::Arm) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_arm_mut(self, i);
            }
        }
    }

    fn visit_assoc_const_mut(&mut self, i: &mut syn::AssocConst) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_assoc_const_mut(self, i);
            }
        }
    }

    fn visit_assoc_type_mut(&mut self, i: &mut syn::AssocType) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_assoc_type_mut(self, i);
            }
        }
    }

    fn visit_attr_style_mut(&mut self, i: &mut syn::AttrStyle) {
        match self.do_work_fallback(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_attr_style_mut(self, i);
            }
        }
    }

    fn visit_attribute_mut(&mut self, i: &mut syn::Attribute) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_attribute_mut(self, i);
            }
        }
    }

    fn visit_attributes_mut(&mut self, i: &mut Vec<syn::Attribute>) {
        for attr in i {
            self.visit_attribute_mut(attr);
        }
    }

    fn visit_bare_fn_arg_mut(&mut self, i: &mut syn::BareFnArg) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_bare_fn_arg_mut(self, i);
            }
        }
    }

    fn visit_bare_variadic_mut(&mut self, i: &mut syn::BareVariadic) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_bare_variadic_mut(self, i);
            }
        }
    }

    fn visit_bin_op_mut(&mut self, i: &mut syn::BinOp) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_bin_op_mut(self, i);
            }
        }
    }

    fn visit_block_mut(&mut self, i: &mut syn::Block) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_block_mut(self, i);
            }
        }
    }

    fn visit_bound_lifetimes_mut(&mut self, i: &mut syn::BoundLifetimes) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_bound_lifetimes_mut(self, i);
            }
        }
    }

    fn visit_captured_param_mut(&mut self, i: &mut syn::CapturedParam) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_captured_param_mut(self, i);
            }
        }
    }

    fn visit_const_param_mut(&mut self, i: &mut syn::ConstParam) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_const_param_mut(self, i);
            }
        }
    }

    fn visit_constraint_mut(&mut self, i: &mut syn::Constraint) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_constraint_mut(self, i);
            }
        }
    }

    fn visit_data_mut(&mut self, i: &mut syn::Data) {
        match self.do_work_fallback(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_data_mut(self, i);
            }
        }
    }

    fn visit_data_enum_mut(&mut self, i: &mut syn::DataEnum) {
        match self.do_work_fallback(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_data_enum_mut(self, i);
            }
        }
    }

    fn visit_data_struct_mut(&mut self, i: &mut syn::DataStruct) {
        match self.do_work_fallback(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_data_struct_mut(self, i);
            }
        }
    }

    fn visit_data_union_mut(&mut self, i: &mut syn::DataUnion) {
        match self.do_work_fallback(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_data_union_mut(self, i);
            }
        }
    }

    fn visit_derive_input_mut(&mut self, i: &mut syn::DeriveInput) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_derive_input_mut(self, i);
            }
        }
    }

    fn visit_expr_mut(&mut self, i: &mut syn::Expr) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_mut(self, i);
            }
        }
    }

    fn visit_expr_array_mut(&mut self, i: &mut syn::ExprArray) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_array_mut(self, i);
            }
        }
    }

    fn visit_expr_assign_mut(&mut self, i: &mut syn::ExprAssign) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_assign_mut(self, i);
            }
        }
    }

    fn visit_expr_async_mut(&mut self, i: &mut syn::ExprAsync) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_async_mut(self, i);
            }
        }
    }

    fn visit_expr_await_mut(&mut self, i: &mut syn::ExprAwait) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_await_mut(self, i);
            }
        }
    }

    fn visit_expr_binary_mut(&mut self, i: &mut syn::ExprBinary) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_binary_mut(self, i);
            }
        }
    }

    fn visit_expr_block_mut(&mut self, i: &mut syn::ExprBlock) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_block_mut(self, i);
            }
        }
    }

    fn visit_expr_break_mut(&mut self, i: &mut syn::ExprBreak) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_break_mut(self, i);
            }
        }
    }

    fn visit_expr_call_mut(&mut self, i: &mut syn::ExprCall) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_call_mut(self, i);
            }
        }
    }

    fn visit_expr_cast_mut(&mut self, i: &mut syn::ExprCast) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_cast_mut(self, i);
            }
        }
    }

    fn visit_expr_closure_mut(&mut self, i: &mut syn::ExprClosure) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_closure_mut(self, i);
            }
        }
    }

    fn visit_expr_const_mut(&mut self, i: &mut syn::ExprConst) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_const_mut(self, i);
            }
        }
    }

    fn visit_expr_continue_mut(&mut self, i: &mut syn::ExprContinue) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_continue_mut(self, i);
            }
        }
    }

    fn visit_expr_field_mut(&mut self, i: &mut syn::ExprField) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_field_mut(self, i);
            }
        }
    }

    fn visit_expr_for_loop_mut(&mut self, i: &mut syn::ExprForLoop) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_for_loop_mut(self, i);
            }
        }
    }

    fn visit_expr_group_mut(&mut self, i: &mut syn::ExprGroup) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_group_mut(self, i);
            }
        }
    }

    fn visit_expr_if_mut(&mut self, i: &mut syn::ExprIf) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_if_mut(self, i);
            }
        }
    }

    fn visit_expr_index_mut(&mut self, i: &mut syn::ExprIndex) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_index_mut(self, i);
            }
        }
    }

    fn visit_expr_infer_mut(&mut self, i: &mut syn::ExprInfer) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_infer_mut(self, i);
            }
        }
    }

    fn visit_expr_let_mut(&mut self, i: &mut syn::ExprLet) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_let_mut(self, i);
            }
        }
    }

    fn visit_expr_lit_mut(&mut self, i: &mut syn::ExprLit) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_lit_mut(self, i);
            }
        }
    }

    fn visit_expr_loop_mut(&mut self, i: &mut syn::ExprLoop) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_loop_mut(self, i);
            }
        }
    }

    fn visit_expr_macro_mut(&mut self, i: &mut syn::ExprMacro) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_macro_mut(self, i);
            }
        }
    }

    fn visit_expr_match_mut(&mut self, i: &mut syn::ExprMatch) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_match_mut(self, i);
            }
        }
    }

    fn visit_expr_method_call_mut(&mut self, i: &mut syn::ExprMethodCall) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_method_call_mut(self, i);
            }
        }
    }

    fn visit_expr_paren_mut(&mut self, i: &mut syn::ExprParen) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_paren_mut(self, i);
            }
        }
    }

    fn visit_expr_path_mut(&mut self, i: &mut syn::ExprPath) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_path_mut(self, i);
            }
        }
    }

    fn visit_expr_range_mut(&mut self, i: &mut syn::ExprRange) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_range_mut(self, i);
            }
        }
    }

    fn visit_expr_raw_addr_mut(&mut self, i: &mut syn::ExprRawAddr) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_raw_addr_mut(self, i);
            }
        }
    }

    fn visit_expr_reference_mut(&mut self, i: &mut syn::ExprReference) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_reference_mut(self, i);
            }
        }
    }

    fn visit_expr_repeat_mut(&mut self, i: &mut syn::ExprRepeat) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_repeat_mut(self, i);
            }
        }
    }

    fn visit_expr_return_mut(&mut self, i: &mut syn::ExprReturn) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_return_mut(self, i);
            }
        }
    }

    fn visit_expr_struct_mut(&mut self, i: &mut syn::ExprStruct) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_struct_mut(self, i);
            }
        }
    }

    fn visit_expr_try_mut(&mut self, i: &mut syn::ExprTry) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_try_mut(self, i);
            }
        }
    }

    fn visit_expr_try_block_mut(&mut self, i: &mut syn::ExprTryBlock) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_try_block_mut(self, i);
            }
        }
    }

    fn visit_expr_tuple_mut(&mut self, i: &mut syn::ExprTuple) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_tuple_mut(self, i);
            }
        }
    }

    fn visit_expr_unary_mut(&mut self, i: &mut syn::ExprUnary) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_unary_mut(self, i);
            }
        }
    }

    fn visit_expr_unsafe_mut(&mut self, i: &mut syn::ExprUnsafe) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_unsafe_mut(self, i);
            }
        }
    }

    fn visit_expr_while_mut(&mut self, i: &mut syn::ExprWhile) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_while_mut(self, i);
            }
        }
    }

    fn visit_expr_yield_mut(&mut self, i: &mut syn::ExprYield) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_expr_yield_mut(self, i);
            }
        }
    }

    fn visit_field_mut(&mut self, i: &mut syn::Field) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_field_mut(self, i);
            }
        }
    }

    fn visit_field_mutability_mut(&mut self, i: &mut syn::FieldMutability) {
        match self.do_work_fallback(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_field_mutability_mut(self, i);
            }
        }
    }

    fn visit_field_pat_mut(&mut self, i: &mut syn::FieldPat) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_field_pat_mut(self, i);
            }
        }
    }

    fn visit_field_value_mut(&mut self, i: &mut syn::FieldValue) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_field_value_mut(self, i);
            }
        }
    }

    fn visit_fields_mut(&mut self, i: &mut syn::Fields) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_fields_mut(self, i);
            }
        }
    }

    fn visit_fields_named_mut(&mut self, i: &mut syn::FieldsNamed) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_fields_named_mut(self, i);
            }
        }
    }

    fn visit_fields_unnamed_mut(&mut self, i: &mut syn::FieldsUnnamed) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_fields_unnamed_mut(self, i);
            }
        }
    }

    fn visit_file_mut(&mut self, i: &mut syn::File) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_file_mut(self, i);
            }
        }
    }

    fn visit_fn_arg_mut(&mut self, i: &mut syn::FnArg) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_fn_arg_mut(self, i);
            }
        }
    }

    fn visit_foreign_item_mut(&mut self, i: &mut syn::ForeignItem) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_foreign_item_mut(self, i);
            }
        }
    }

    fn visit_foreign_item_fn_mut(&mut self, i: &mut syn::ForeignItemFn) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_foreign_item_fn_mut(self, i);
            }
        }
    }

    fn visit_foreign_item_macro_mut(&mut self, i: &mut syn::ForeignItemMacro) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_foreign_item_macro_mut(self, i);
            }
        }
    }

    fn visit_foreign_item_static_mut(&mut self, i: &mut syn::ForeignItemStatic) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_foreign_item_static_mut(self, i);
            }
        }
    }

    fn visit_foreign_item_type_mut(&mut self, i: &mut syn::ForeignItemType) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_foreign_item_type_mut(self, i);
            }
        }
    }

    fn visit_generic_argument_mut(&mut self, i: &mut syn::GenericArgument) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_generic_argument_mut(self, i);
            }
        }
    }

    fn visit_generic_param_mut(&mut self, i: &mut syn::GenericParam) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_generic_param_mut(self, i);
            }
        }
    }

    fn visit_generics_mut(&mut self, i: &mut syn::Generics) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_generics_mut(self, i);
            }
        }
    }
    fn visit_ident_mut(&mut self, i: &mut proc_macro2::Ident) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_ident_mut(self, i);
            }
        }
    }

    fn visit_impl_item_mut(&mut self, i: &mut syn::ImplItem) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_impl_item_mut(self, i);
            }
        }
    }

    fn visit_impl_item_const_mut(&mut self, i: &mut syn::ImplItemConst) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_impl_item_const_mut(self, i);
            }
        }
    }

    fn visit_impl_item_fn_mut(&mut self, i: &mut syn::ImplItemFn) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_impl_item_fn_mut(self, i);
            }
        }
    }

    fn visit_impl_item_macro_mut(&mut self, i: &mut syn::ImplItemMacro) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_impl_item_macro_mut(self, i);
            }
        }
    }

    fn visit_impl_item_type_mut(&mut self, i: &mut syn::ImplItemType) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_impl_item_type_mut(self, i);
            }
        }
    }

    fn visit_impl_restriction_mut(&mut self, i: &mut syn::ImplRestriction) {
        match self.do_work_fallback(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_impl_restriction_mut(self, i);
            }
        }
    }

    fn visit_index_mut(&mut self, i: &mut syn::Index) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_index_mut(self, i);
            }
        }
    }

    fn visit_item_mut(&mut self, i: &mut syn::Item) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_item_mut(self, i);
            }
        }
    }

    fn visit_item_const_mut(&mut self, i: &mut syn::ItemConst) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_item_const_mut(self, i);
            }
        }
    }

    fn visit_item_enum_mut(&mut self, i: &mut syn::ItemEnum) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_item_enum_mut(self, i);
            }
        }
    }

    fn visit_item_extern_crate_mut(&mut self, i: &mut syn::ItemExternCrate) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_item_extern_crate_mut(self, i);
            }
        }
    }

    fn visit_item_fn_mut(&mut self, i: &mut syn::ItemFn) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_item_fn_mut(self, i);
            }
        }
    }

    fn visit_item_foreign_mod_mut(&mut self, i: &mut syn::ItemForeignMod) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_item_foreign_mod_mut(self, i);
            }
        }
    }

    fn visit_item_impl_mut(&mut self, i: &mut syn::ItemImpl) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_item_impl_mut(self, i);
            }
        }
    }

    fn visit_item_macro_mut(&mut self, i: &mut syn::ItemMacro) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_item_macro_mut(self, i);
            }
        }
    }

    fn visit_item_mod_mut(&mut self, i: &mut syn::ItemMod) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_item_mod_mut(self, i);
            }
        }
    }

    fn visit_item_static_mut(&mut self, i: &mut syn::ItemStatic) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_item_static_mut(self, i);
            }
        }
    }

    fn visit_item_struct_mut(&mut self, i: &mut syn::ItemStruct) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_item_struct_mut(self, i);
            }
        }
    }

    fn visit_item_trait_mut(&mut self, i: &mut syn::ItemTrait) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_item_trait_mut(self, i);
            }
        }
    }

    fn visit_item_trait_alias_mut(&mut self, i: &mut syn::ItemTraitAlias) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_item_trait_alias_mut(self, i);
            }
        }
    }

    fn visit_item_type_mut(&mut self, i: &mut syn::ItemType) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_item_type_mut(self, i);
            }
        }
    }

    fn visit_item_union_mut(&mut self, i: &mut syn::ItemUnion) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_item_union_mut(self, i);
            }
        }
    }

    fn visit_item_use_mut(&mut self, i: &mut syn::ItemUse) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_item_use_mut(self, i);
            }
        }
    }

    fn visit_label_mut(&mut self, i: &mut syn::Label) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_label_mut(self, i);
            }
        }
    }
    fn visit_lifetime_mut(&mut self, i: &mut syn::Lifetime) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_lifetime_mut(self, i);
            }
        }
    }

    fn visit_lifetime_param_mut(&mut self, i: &mut syn::LifetimeParam) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_lifetime_param_mut(self, i);
            }
        }
    }
    fn visit_lit_mut(&mut self, i: &mut syn::Lit) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_lit_mut(self, i);
            }
        }
    }
    fn visit_lit_bool_mut(&mut self, i: &mut syn::LitBool) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_lit_bool_mut(self, i);
            }
        }
    }
    fn visit_lit_byte_mut(&mut self, i: &mut syn::LitByte) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_lit_byte_mut(self, i);
            }
        }
    }
    fn visit_lit_byte_str_mut(&mut self, i: &mut syn::LitByteStr) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_lit_byte_str_mut(self, i);
            }
        }
    }
    fn visit_lit_cstr_mut(&mut self, i: &mut syn::LitCStr) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_lit_cstr_mut(self, i);
            }
        }
    }
    fn visit_lit_char_mut(&mut self, i: &mut syn::LitChar) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_lit_char_mut(self, i);
            }
        }
    }
    fn visit_lit_float_mut(&mut self, i: &mut syn::LitFloat) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_lit_float_mut(self, i);
            }
        }
    }
    fn visit_lit_int_mut(&mut self, i: &mut syn::LitInt) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_lit_int_mut(self, i);
            }
        }
    }
    fn visit_lit_str_mut(&mut self, i: &mut syn::LitStr) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_lit_str_mut(self, i);
            }
        }
    }

    fn visit_local_mut(&mut self, i: &mut syn::Local) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_local_mut(self, i);
            }
        }
    }

    fn visit_local_init_mut(&mut self, i: &mut syn::LocalInit) {
        match self.do_work_fallback(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_local_init_mut(self, i);
            }
        }
    }

    fn visit_macro_mut(&mut self, i: &mut syn::Macro) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_macro_mut(self, i);
            }
        }
    }

    fn visit_macro_delimiter_mut(&mut self, i: &mut syn::MacroDelimiter) {
        match self.do_work_fallback(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_macro_delimiter_mut(self, i);
            }
        }
    }

    fn visit_member_mut(&mut self, i: &mut syn::Member) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_member_mut(self, i);
            }
        }
    }

    fn visit_meta_mut(&mut self, i: &mut syn::Meta) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_meta_mut(self, i);
            }
        }
    }

    fn visit_meta_list_mut(&mut self, i: &mut syn::MetaList) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_meta_list_mut(self, i);
            }
        }
    }

    fn visit_meta_name_value_mut(&mut self, i: &mut syn::MetaNameValue) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_meta_name_value_mut(self, i);
            }
        }
    }

    fn visit_parenthesized_generic_arguments_mut(
        &mut self,
        i: &mut syn::ParenthesizedGenericArguments,
    ) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_parenthesized_generic_arguments_mut(self, i);
            }
        }
    }

    fn visit_pat_mut(&mut self, i: &mut syn::Pat) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_pat_mut(self, i);
            }
        }
    }

    fn visit_pat_ident_mut(&mut self, i: &mut syn::PatIdent) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_pat_ident_mut(self, i);
            }
        }
    }

    fn visit_pat_or_mut(&mut self, i: &mut syn::PatOr) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_pat_or_mut(self, i);
            }
        }
    }

    fn visit_pat_paren_mut(&mut self, i: &mut syn::PatParen) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_pat_paren_mut(self, i);
            }
        }
    }

    fn visit_pat_reference_mut(&mut self, i: &mut syn::PatReference) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_pat_reference_mut(self, i);
            }
        }
    }

    fn visit_pat_rest_mut(&mut self, i: &mut syn::PatRest) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_pat_rest_mut(self, i);
            }
        }
    }

    fn visit_pat_slice_mut(&mut self, i: &mut syn::PatSlice) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_pat_slice_mut(self, i);
            }
        }
    }

    fn visit_pat_struct_mut(&mut self, i: &mut syn::PatStruct) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_pat_struct_mut(self, i);
            }
        }
    }

    fn visit_pat_tuple_mut(&mut self, i: &mut syn::PatTuple) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_pat_tuple_mut(self, i);
            }
        }
    }

    fn visit_pat_tuple_struct_mut(&mut self, i: &mut syn::PatTupleStruct) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_pat_tuple_struct_mut(self, i);
            }
        }
    }

    fn visit_pat_type_mut(&mut self, i: &mut syn::PatType) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_pat_type_mut(self, i);
            }
        }
    }

    fn visit_pat_wild_mut(&mut self, i: &mut syn::PatWild) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_pat_wild_mut(self, i);
            }
        }
    }

    fn visit_path_mut(&mut self, i: &mut syn::Path) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_path_mut(self, i);
            }
        }
    }

    fn visit_path_arguments_mut(&mut self, i: &mut syn::PathArguments) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_path_arguments_mut(self, i);
            }
        }
    }

    fn visit_path_segment_mut(&mut self, i: &mut syn::PathSegment) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_path_segment_mut(self, i);
            }
        }
    }

    fn visit_pointer_mutability_mut(&mut self, i: &mut syn::PointerMutability) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_pointer_mutability_mut(self, i);
            }
        }
    }

    fn visit_precise_capture_mut(&mut self, i: &mut syn::PreciseCapture) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_precise_capture_mut(self, i);
            }
        }
    }

    fn visit_predicate_lifetime_mut(&mut self, i: &mut syn::PredicateLifetime) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_predicate_lifetime_mut(self, i);
            }
        }
    }

    fn visit_predicate_type_mut(&mut self, i: &mut syn::PredicateType) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_predicate_type_mut(self, i);
            }
        }
    }

    fn visit_qself_mut(&mut self, i: &mut syn::QSelf) {
        match self.do_work_fallback(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_qself_mut(self, i);
            }
        }
    }

    fn visit_range_limits_mut(&mut self, i: &mut syn::RangeLimits) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_range_limits_mut(self, i);
            }
        }
    }

    fn visit_receiver_mut(&mut self, i: &mut syn::Receiver) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_receiver_mut(self, i);
            }
        }
    }

    fn visit_return_type_mut(&mut self, i: &mut syn::ReturnType) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_return_type_mut(self, i);
            }
        }
    }

    fn visit_signature_mut(&mut self, i: &mut syn::Signature) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_signature_mut(self, i);
            }
        }
    }

    fn visit_static_mutability_mut(&mut self, i: &mut syn::StaticMutability) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_static_mutability_mut(self, i);
            }
        }
    }

    fn visit_stmt_mut(&mut self, i: &mut syn::Stmt) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_stmt_mut(self, i);
            }
        }
    }

    fn visit_stmt_macro_mut(&mut self, i: &mut syn::StmtMacro) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_stmt_macro_mut(self, i);
            }
        }
    }

    fn visit_trait_bound_mut(&mut self, i: &mut syn::TraitBound) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_trait_bound_mut(self, i);
            }
        }
    }

    fn visit_trait_bound_modifier_mut(&mut self, i: &mut syn::TraitBoundModifier) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_trait_bound_modifier_mut(self, i);
            }
        }
    }

    fn visit_trait_item_mut(&mut self, i: &mut syn::TraitItem) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_trait_item_mut(self, i);
            }
        }
    }

    fn visit_trait_item_const_mut(&mut self, i: &mut syn::TraitItemConst) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_trait_item_const_mut(self, i);
            }
        }
    }

    fn visit_trait_item_fn_mut(&mut self, i: &mut syn::TraitItemFn) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_trait_item_fn_mut(self, i);
            }
        }
    }

    fn visit_trait_item_macro_mut(&mut self, i: &mut syn::TraitItemMacro) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_trait_item_macro_mut(self, i);
            }
        }
    }

    fn visit_trait_item_type_mut(&mut self, i: &mut syn::TraitItemType) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_trait_item_type_mut(self, i);
            }
        }
    }

    fn visit_type_mut(&mut self, i: &mut syn::Type) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_type_mut(self, i);
            }
        }
    }

    fn visit_type_array_mut(&mut self, i: &mut syn::TypeArray) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_type_array_mut(self, i);
            }
        }
    }

    fn visit_type_bare_fn_mut(&mut self, i: &mut syn::TypeBareFn) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_type_bare_fn_mut(self, i);
            }
        }
    }

    fn visit_type_group_mut(&mut self, i: &mut syn::TypeGroup) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_type_group_mut(self, i);
            }
        }
    }

    fn visit_type_impl_trait_mut(&mut self, i: &mut syn::TypeImplTrait) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_type_impl_trait_mut(self, i);
            }
        }
    }

    fn visit_type_infer_mut(&mut self, i: &mut syn::TypeInfer) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_type_infer_mut(self, i);
            }
        }
    }

    fn visit_type_macro_mut(&mut self, i: &mut syn::TypeMacro) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_type_macro_mut(self, i);
            }
        }
    }

    fn visit_type_never_mut(&mut self, i: &mut syn::TypeNever) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_type_never_mut(self, i);
            }
        }
    }

    fn visit_type_param_mut(&mut self, i: &mut syn::TypeParam) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_type_param_mut(self, i);
            }
        }
    }

    fn visit_type_param_bound_mut(&mut self, i: &mut syn::TypeParamBound) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_type_param_bound_mut(self, i);
            }
        }
    }

    fn visit_type_paren_mut(&mut self, i: &mut syn::TypeParen) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_type_paren_mut(self, i);
            }
        }
    }

    fn visit_type_path_mut(&mut self, i: &mut syn::TypePath) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_type_path_mut(self, i);
            }
        }
    }

    fn visit_type_ptr_mut(&mut self, i: &mut syn::TypePtr) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_type_ptr_mut(self, i);
            }
        }
    }

    fn visit_type_reference_mut(&mut self, i: &mut syn::TypeReference) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_type_reference_mut(self, i);
            }
        }
    }

    fn visit_type_slice_mut(&mut self, i: &mut syn::TypeSlice) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_type_slice_mut(self, i);
            }
        }
    }

    fn visit_type_trait_object_mut(&mut self, i: &mut syn::TypeTraitObject) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_type_trait_object_mut(self, i);
            }
        }
    }

    fn visit_type_tuple_mut(&mut self, i: &mut syn::TypeTuple) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_type_tuple_mut(self, i);
            }
        }
    }

    fn visit_un_op_mut(&mut self, i: &mut syn::UnOp) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_un_op_mut(self, i);
            }
        }
    }

    fn visit_use_glob_mut(&mut self, i: &mut syn::UseGlob) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_use_glob_mut(self, i);
            }
        }
    }

    fn visit_use_group_mut(&mut self, i: &mut syn::UseGroup) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_use_group_mut(self, i);
            }
        }
    }

    fn visit_use_name_mut(&mut self, i: &mut syn::UseName) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_use_name_mut(self, i);
            }
        }
    }

    fn visit_use_path_mut(&mut self, i: &mut syn::UsePath) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_use_path_mut(self, i);
            }
        }
    }

    fn visit_use_rename_mut(&mut self, i: &mut syn::UseRename) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_use_rename_mut(self, i);
            }
        }
    }

    fn visit_use_tree_mut(&mut self, i: &mut syn::UseTree) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_use_tree_mut(self, i);
            }
        }
    }

    fn visit_variadic_mut(&mut self, i: &mut syn::Variadic) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_variadic_mut(self, i);
            }
        }
    }

    fn visit_variant_mut(&mut self, i: &mut syn::Variant) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_variant_mut(self, i);
            }
        }
    }

    fn visit_vis_restricted_mut(&mut self, i: &mut syn::VisRestricted) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_vis_restricted_mut(self, i);
            }
        }
    }

    fn visit_visibility_mut(&mut self, i: &mut syn::Visibility) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_visibility_mut(self, i);
            }
        }
    }

    fn visit_where_clause_mut(&mut self, i: &mut syn::WhereClause) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_where_clause_mut(self, i);
            }
        }
    }

    fn visit_where_predicate_mut(&mut self, i: &mut syn::WherePredicate) {
        match self.do_work(i) {
            Some(_) => {
                return;
            }
            None => {
                syn::visit_mut::visit_where_predicate_mut(self, i);
            }
        }
    }
}
