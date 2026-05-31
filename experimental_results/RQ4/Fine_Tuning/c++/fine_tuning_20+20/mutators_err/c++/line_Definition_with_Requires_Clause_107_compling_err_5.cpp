//header file
#pragma once
#include "Mutator_base.h"

/**
 * Out-of-line_Definition_with_Requires_Clause_107
 */ 
class MutatorFrontendAction_107 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(107)

private:
    class MutatorASTConsumer_107 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_107(Rewriter &R) : TheRewriter(R) {}
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
        const FunctionDecl *cur_func = nullptr;
    };
};

//source file
#include "../include/Out-of-line_Definition_with_Requires_Clause_107.h"

// ========================================================================================================
#define MUT107_OUTPUT 1

void MutatorFrontendAction_107::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlineDefinitionExternallyVisible() == false)
        return;
      if (FD->isTemplated() == false)
        return;
      if (FD->getKind() != Decl::Kind::Function)
        return;
      if (FD->getDescribedFunctionTemplate() == nullptr)
        return;
      if (FD->getTemplateSpecializationKind() !=
          TemplateSpecializationKind::TSK_Undeclared)
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isFunctionTemplateSpecialization())
        return;
      if (FD->isDefinedOutsideFunctionOrMethod())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->doesThisDeclarationHaveABody() == false)
        return;
      if (FD->hasAttrs())
        return;
      if (FD->hasPrototype() == false)
        return;
      if (FD->isPure())
        return;
      if (FD->isLateTemplateParsed() == true)
        return;
      if (FD->isLocalExternDecl())
        return;
      if (FD->isInlined())
        return;
      if (FD->isMultiVersion())
        return;
      if (FD->isInlineBuiltinDeclaration())
        return;
      if (FD->isInlineSpecified())
        return;
      if (FD->isVariadic())
        return;
      if (FD->isDeleted())
        return;
      if (FD->isGlobal())
        return;
      if (FD->isInlined())
        return;
      if (FD->hasBody() == false)
        return;
      if (FD->isOutOfLine() == false)
        return;
      if (FD->isDefinedOutsideFunctionOrMethod() == false)
        return;
      if (FD