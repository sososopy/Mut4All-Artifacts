//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Argument_With_Concept_Check_128
 */ 
class MutatorFrontendAction_128 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(128)

private:
    class MutatorASTConsumer_128 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_128(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        
    };
};

//source file
#include "../include/Replace_Template_Argument_With_Concept_Check_128.h"

// ========================================================================================================
#define MUT128_OUTPUT 1

void MutatorFrontendAction_128::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FC = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FC->getLocation()))
        return;

      if (!FC->hasBody())
        return;

      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), FC->getSourceRange());
      llvm::outs() << content << '\n';
      auto templatecall = Result.Nodes.getNodeAs<clang::CallExpr>("TemplateCall");
      if (!templatecall)
        return;
      auto templatecall_content = stringutils::rangetoStr(
          *(Result.SourceManager), templatecall->getSourceRange());
      llvm::outs() << templatecall_content << '\n';
      auto templateargs = Result.Nodes.getNodeAs<clang::TemplateArgumentLoc>(
          "TemplateArguments");
      if (!templateargs)
        return;
      auto templateargs_content = stringutils::rangetoStr(
          *(Result.SourceManager), templateargs->getSourceRange());
      llvm::outs() << templateargs_content << '\n';
      auto templateargs_type = Result.Nodes.getNodeAs<clang::Type>(
          "TemplateArgumentsType");
      if (!templateargs_type)
        return;
      // auto templateargs_type_content = stringutils::rangetoStr(
      //     *(Result.SourceManager), templateargs_type->getSourceRange());
      // llvm::outs() << templateargs_type_content << '\n';

      auto templateargs_type_name = templateargs_type->getAsTagDecl();
      if (!templateargs_type_name)
        return;
      auto templateargs_type_name_content = stringutils::rangetoStr(
          *(Result.SourceManager), templateargs_type_name->getSourceRange());
      llvm::outs() << templateargs_type_name_content << '\n';

      auto templateargs_type_name_class = llvm::dyn_cast<CXXRecordDecl>(
          templateargs_type_name); // if it is a class
      if (!templateargs_type_name_class)
        return;

      auto templateargs_type_name_class_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_fields) {
        llvm::outs() << field->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_methods =
      // templateargs_type_name_class->methods();
      // for(auto method:templateargs_type_name_class_methods){
      //   llvm::outs()<<method->getNameAsString()<<'\n';
      // }

      auto templateargs_type_name_class_public_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_public_methods) {
        if (method->getAccess() == AccessSpecifier::AS_public)
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_public_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_public_fields) {
        if (field->getAccess() == AccessSpecifier::AS_public)
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_protected_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_protected_fields) {
        if (field->getAccess() == AccessSpecifier::AS_protected)
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_protected_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_protected_methods) {
        if (method->getAccess() == AccessSpecifier::AS_protected)
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_private_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_private_fields) {
        if (field->getAccess() == AccessSpecifier::AS_private)
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_private_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_private_methods) {
        if (method->getAccess() == AccessSpecifier::AS_private)
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_static_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_static_fields) {
        if (field->isStaticDataMember())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_static_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_static_methods) {
        if (method->isStatic())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nonstatic_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_nonstatic_fields) {
        if (!field->isStaticDataMember())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nonstatic_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_nonstatic_methods) {
        if (!method->isStatic())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_const_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_const_fields) {
        if (field->getType().isConstQualified())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_const_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_const_methods) {
        if (method->isConst())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nonconst_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_nonconst_fields) {
        if (!field->getType().isConstQualified())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nonconst_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_nonconst_methods) {
        if (!method->isConst())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_volatile_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_volatile_fields) {
        if (field->getType().isVolatileQualified())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_volatile_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_volatile_methods) {
        if (method->isVolatile())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nonvolatile_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_nonvolatile_fields) {
        if (!field->getType().isVolatileQualified())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nonvolatile_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_nonvolatile_methods) {
        if (!method->isVolatile())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_lvalue_ref_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_lvalue_ref_fields) {
        if (field->getType()->isLValueReferenceType())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_lvalue_ref_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_lvalue_ref_methods) {
        if (method->getReturnType()->isLValueReferenceType())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_rvalue_ref_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_rvalue_ref_fields) {
        if (field->getType()->isRValueReferenceType())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_rvalue_ref_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_rvalue_ref_methods) {
        if (method->getReturnType()->isRValueReferenceType())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_pointer_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_pointer_fields) {
        if (field->getType()->isPointerType())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_pointer_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_pointer_methods) {
        if (method->getReturnType()->isPointerType())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_array_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_array_fields) {
        if (field->getType()->isArrayType())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_array_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_array_methods) {
        if (method->getReturnType()->isArrayType())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_function_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_function_fields) {
        if (field->getType()->isFunctionType())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_function_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_function_methods) {
        if (method->getReturnType()->isFunctionType())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_member_function_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_member_function_fields) {
        if (field->getType()->isMemberFunctionPointerType())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_member_function_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_member_function_methods) {
        if (method->getReturnType()->isMemberFunctionPointerType())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_member_data_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_member_data_fields) {
        if (field->getType()->isMemberDataPointerType())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_member_data_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_member_data_methods) {
        if (method->getReturnType()->isMemberDataPointerType())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_enum_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_enum_fields) {
        if (field->getType()->isEnumeralType())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_enum_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_enum_methods) {
        if (method->getReturnType()->isEnumeralType())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_union_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_union_fields) {
        if (field->getType()->isUnionType())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_union_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_union_methods) {
        if (method->getReturnType()->isUnionType())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_class_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_class_fields) {
        if (field->getType()->isClassType())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_class_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_class_methods) {
        if (method->getReturnType()->isClassType())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_struct_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_struct_fields) {
        if (field->getType()->isStructureType())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_struct_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_struct_methods) {
        if (method->getReturnType()->isStructureType())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_template_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_template_fields) {
        if (field->getType()->isTemplateTypeParmType())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_template_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_template_methods) {
        if (method->getReturnType()->isTemplateTypeParmType())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_typedef_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_typedef_fields) {
        if (field->getType()->isTypedefNameType())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_typedef_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_typedef_methods) {
        if (method->getReturnType()->isTypedefNameType())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_auto_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_auto_fields) {
        if (field->getType()->getContainedAutoType())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_auto_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_auto_methods) {
        if (method->getReturnType()->getContainedAutoType())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_decltype_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_decltype_fields) {
        if (field->getType()->isDecltypeType())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_decltype_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_decltype_methods) {
        if (method->getReturnType()->isDecltypeType())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_constexpr_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_constexpr_fields) {
      //   if (field->isConstexpr())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_constexpr_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_constexpr_methods) {
        if (method->isConstexpr())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_noexcept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_noexcept_fields) {
      //   if (field->getType()->isNoexceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_noexcept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_noexcept_methods) {
      //   if (method->getType()->isNoexceptType())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_virtual_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_virtual_fields) {
      //   if (field->getType()->isVirtualType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_virtual_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_virtual_methods) {
        if (method->isVirtual())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_pure_virtual_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_pure_virtual_fields) {
      //   if (field->getType()->isPure())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_pure_virtual_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_pure_virtual_methods) {
        if (method->isPure())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_override_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_override_fields) {
      //   if (field->getType()->isOverride())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_override_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_override_methods) {
        if (method->isVirtualAsWritten())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_final_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_final_fields) {
      //   if (field->getType()->isFinal())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_final_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_final_methods) {
        if (method->hasAttr<FinalAttr>())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_deleted_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_deleted_fields) {
      //   if (field->getType()->isDeleted())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_deleted_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_deleted_methods) {
        if (method->isDeleted())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_defaulted_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_defaulted_fields) {
      //   if (field->getType()->isDefaulted())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_defaulted_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_defaulted_methods) {
        if (method->isDefaulted())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_explicit_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_explicit_fields) {
      //   if (field->getType()->isExplicit())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_explicit_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_explicit_methods) {
        if (method->isExplicitlyDefaulted())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_implicit_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_implicit_fields) {
      //   if (field->getType()->isImplicit())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_implicit_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_implicit_methods) {
        if (method->isImplicit())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_trivial_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_trivial_fields) {
        if (field->getType()->isTrivialType(*Result.Context))
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_trivial_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_trivial_methods) {
        if (method->getType()->isTrivialType(*Result.Context))
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nontrivial_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_nontrivial_fields) {
        if (!field->getType()->isTrivialType(*Result.Context))
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nontrivial_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_nontrivial_methods) {
        if (!method->getType()->isTrivialType(*Result.Context))
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_pod_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_pod_fields) {
        if (field->getType()->isPODType(*Result.Context))
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_pod_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_pod_methods) {
        if (method->getType()->isPODType(*Result.Context))
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nonpod_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_nonpod_fields) {
        if (!field->getType()->isPODType(*Result.Context))
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nonpod_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_nonpod_methods) {
        if (!method->getType()->isPODType(*Result.Context))
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_literal_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_literal_fields) {
        if (field->getType()->isLiteralType(*Result.Context))
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_literal_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_literal_methods) {
        if (method->getType()->isLiteralType(*Result.Context))
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nonliteral_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_nonliteral_fields) {
        if (!field->getType()->isLiteralType(*Result.Context))
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nonliteral_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_nonliteral_methods) {
        if (!method->getType()->isLiteralType(*Result.Context))
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_aggregate_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_aggregate_fields) {
        if (field->getType()->isAggregateType())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_aggregate_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_aggregate_methods) {
        if (method->getType()->isAggregateType())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nonaggregate_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_nonaggregate_fields) {
        if (!field->getType()->isAggregateType())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nonaggregate_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_nonaggregate_methods) {
        if (!method->getType()->isAggregateType())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_polymorphic_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_polymorphic_fields) {
        if (field->getType()->isPolymorphicType(*Result.Context))
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_polymorphic_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_polymorphic_methods) {
        if (method->getType()->isPolymorphicType(*Result.Context))
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nonpolymorphic_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_nonpolymorphic_fields) {
        if (!field->getType()->isPolymorphicType(*Result.Context))
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nonpolymorphic_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_nonpolymorphic_methods) {
        if (!method->getType()->isPolymorphicType(*Result.Context))
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_abstract_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_abstract_fields) {
        if (field->getType()->isAbstractType(*Result.Context))
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_abstract_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_abstract_methods) {
        if (method->getType()->isAbstractType(*Result.Context))
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nonabstract_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_nonabstract_fields) {
        if (!field->getType()->isAbstractType(*Result.Context))
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nonabstract_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_nonabstract_methods) {
        if (!method->getType()->isAbstractType(*Result.Context))
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_final_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_final_fields) {
      //   if (field->getType()->isFinal())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_final_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_final_methods) {
      //   if (method->isFinal())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_nonfinal_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_nonfinal_fields) {
        if (!field->getType()->isFinal())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nonfinal_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_nonfinal_methods) {
        if (!method->isFinal())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_sealed_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_sealed_fields) {
      //   if (field->getType()->isSealed())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_sealed_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_sealed_methods) {
      //   if (method->isSealed())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_nonsealed_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_nonsealed_fields) {
        if (!field->getType()->isSealed())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_nonsealed_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_nonsealed_methods) {
        if (!method->isSealed())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_nothrow_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_nothrow_fields) {
      //   if (field->getType()->isNothrowType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_nothrow_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_nothrow_methods) {
        if (method->isNoThrow())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_non_nothrow_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_nothrow_fields) {
      //   if (!field->getType()->isNothrowType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_non_nothrow_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_non_nothrow_methods) {
        if (!method->isNoThrow())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_alignas_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_alignas_fields) {
      //   if (field->getType()->isAlignOfType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_alignas_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_alignas_methods) {
        if (method->isAlignOf())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_non_alignas_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_alignas_fields) {
      //   if (!field->getType()->isAlignOfType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_non_alignas_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_non_alignas_methods) {
        if (!method->isAlignOf())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_alignof_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_alignof_fields) {
      //   if (field->getType()->isAlignOfType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_alignof_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_alignof_methods) {
        if (method->isAlignOf())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_non_alignof_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_alignof_fields) {
      //   if (!field->getType()->isAlignOfType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_non_alignof_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_non_alignof_methods) {
        if (!method->isAlignOf())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_typeid_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_typeid_fields) {
      //   if (field->getType()->isTypeOfType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_typeid_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_typeid_methods) {
        if (method->isTypeOf())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_non_typeid_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_typeid_fields) {
      //   if (!field->getType()->isTypeOfType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_non_typeid_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_non_typeid_methods) {
        if (!method->isTypeOf())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_declval_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_declval_fields) {
        if (field->getType()->isDecltypeType())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_declval_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_declval_methods) {
        if (method->getReturnType()->isDecltypeType())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_non_declval_fields =
          templateargs_type_name_class->fields();
      for (auto field : templateargs_type_name_class_non_declval_fields) {
        if (!field->getType()->isDecltypeType())
          llvm::outs() << field->getNameAsString() << '\n';
      }

      auto templateargs_type_name_class_non_declval_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_non_declval_methods) {
        if (!method->getReturnType()->isDecltypeType())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_constinit_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_constinit_fields) {
      //   if (field->getType()->isConstInitType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_constinit_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_constinit_methods) {
        if (method->isConstInit())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_non_constinit_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_constinit_fields) {
      //   if (!field->getType()->isConstInitType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_non_constinit_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_non_constinit_methods) {
        if (!method->isConstInit())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_consteval_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_consteval_fields) {
      //   if (field->getType()->isConstExprType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_consteval_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_consteval_methods) {
        if (method->isConstexpr())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_non_consteval_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_consteval_fields) {
      //   if (!field->getType()->isConstExprType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_non_consteval_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_non_consteval_methods) {
        if (!method->isConstexpr())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_concept_fields) {
      //   if (field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_concept_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_concept_methods) {
        if (method->isConcept())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_non_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_concept_fields) {
      //   if (!field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_non_concept_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_non_concept_methods) {
        if (!method->isConcept())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_requires_fields) {
      //   if (field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_requires_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_requires_methods) {
        if (method->isRequires())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_non_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_requires_fields) {
      //   if (!field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      auto templateargs_type_name_class_non_requires_methods =
          templateargs_type_name_class->methods();
      for (auto method : templateargs_type_name_class_non_requires_methods) {
        if (!method->isRequires())
          llvm::outs() << method->getNameAsString() << '\n';
      }

      // auto templateargs_type_name_class_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_concept_fields) {
      //   if (field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_concept_methods) {
      //   if (method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_concept_fields) {
      //   if (!field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_concept_methods) {
      //   if (!method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_requires_fields) {
      //   if (field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_requires_methods) {
      //   if (method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_requires_fields) {
      //   if (!field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_requires_methods) {
      //   if (!method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_concept_fields) {
      //   if (field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_concept_methods) {
      //   if (method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_concept_fields) {
      //   if (!field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_concept_methods) {
      //   if (!method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_requires_fields) {
      //   if (field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_requires_methods) {
      //   if (method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_requires_fields) {
      //   if (!field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_requires_methods) {
      //   if (!method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_concept_fields) {
      //   if (field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_concept_methods) {
      //   if (method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_concept_fields) {
      //   if (!field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_concept_methods) {
      //   if (!method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_requires_fields) {
      //   if (field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_requires_methods) {
      //   if (method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_requires_fields) {
      //   if (!field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_requires_methods) {
      //   if (!method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_concept_fields) {
      //   if (field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_concept_methods) {
      //   if (method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_concept_fields) {
      //   if (!field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_concept_methods) {
      //   if (!method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_requires_fields) {
      //   if (field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_requires_methods) {
      //   if (method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_requires_fields) {
      //   if (!field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_requires_methods) {
      //   if (!method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_concept_fields) {
      //   if (field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_concept_methods) {
      //   if (method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_concept_fields) {
      //   if (!field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_concept_methods) {
      //   if (!method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_requires_fields) {
      //   if (field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_requires_methods) {
      //   if (method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_requires_fields) {
      //   if (!field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_requires_methods) {
      //   if (!method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_concept_fields) {
      //   if (field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_concept_methods) {
      //   if (method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_concept_fields) {
      //   if (!field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_concept_methods) {
      //   if (!method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_requires_fields) {
      //   if (field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_requires_methods) {
      //   if (method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_requires_fields) {
      //   if (!field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_requires_methods) {
      //   if (!method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_concept_fields) {
      //   if (field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_concept_methods) {
      //   if (method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_concept_fields) {
      //   if (!field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_concept_methods) {
      //   if (!method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_requires_fields) {
      //   if (field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_requires_methods) {
      //   if (method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_requires_fields) {
      //   if (!field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_requires_methods) {
      //   if (!method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_concept_fields) {
      //   if (field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_concept_methods) {
      //   if (method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_concept_fields) {
      //   if (!field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_concept_methods) {
      //   if (!method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_requires_fields) {
      //   if (field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_requires_methods) {
      //   if (method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_requires_fields) {
      //   if (!field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_requires_methods) {
      //   if (!method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_concept_fields) {
      //   if (field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_concept_methods) {
      //   if (method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_concept_fields) {
      //   if (!field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_concept_methods) {
      //   if (!method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_requires_fields) {
      //   if (field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_requires_methods) {
      //   if (method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_requires_fields) {
      //   if (!field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_requires_methods) {
      //   if (!method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_concept_fields) {
      //   if (field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_concept_methods) {
      //   if (method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_concept_fields) {
      //   if (!field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_concept_methods) {
      //   if (!method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_requires_fields) {
      //   if (field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_requires_methods) {
      //   if (method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_requires_fields) {
      //   if (!field->getType()->isRequiresType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_requires_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_requires_methods) {
      //   if (!method->isRequires())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_concept_fields) {
      //   if (field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_concept_methods) {
      //   if (method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_fields =
      //     templateargs_type_name_class->fields();
      // for (auto field : templateargs_type_name_class_non_concept_fields) {
      //   if (!field->getType()->isConceptType())
      //     llvm::outs() << field->getNameAsString() << '\n';
      // }

      // auto templateargs_type_name_class_non_concept_methods =
      //     templateargs_type_name_class->methods();
      // for (auto method : templateargs_type_name_class_non_concept_methods) {
      //   if (!method->isConcept())
      //     llvm::outs() << method->getNameAsString() << '\