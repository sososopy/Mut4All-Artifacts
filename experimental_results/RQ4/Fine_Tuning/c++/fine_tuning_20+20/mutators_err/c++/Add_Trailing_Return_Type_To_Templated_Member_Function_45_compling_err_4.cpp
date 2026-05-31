//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Trailing_Return_Type_To_Templated_Member_Function_45
 */ 
class MutatorFrontendAction_45 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(45)

private:
    class MutatorASTConsumer_45 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_45(Rewriter &R) : TheRewriter(R) {}
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
        const clang::TemplateTypeParmDecl *return_type = nullptr;
    };
};

//source file
#include "../include/Add_Trailing_Return_Type_To_Templated_Member_Function_45.h"

// ========================================================================================================
#define MUT45_OUTPUT 1

void MutatorFrontendAction_45::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->hasTrailingReturnType())
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_Ordinary))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_OrdinaryFriend))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_NonMemberOperator))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_LocalExtern))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_TagFriend))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_Using))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_NonMemberOperator))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_LocalExtern))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_TagFriend))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_Using))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_UsingShadow))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_Concept))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_Module))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_Type))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_Tag))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_Member))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_Namespace))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_Concept))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_Module))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_Type))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_Tag))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_Member))
        return;
      if (!MT->isInIdentifierNamespace(Decl::IDNS_Namespace))
        return;
      if (MT->isLambdaStaticInvoker())
        return;
      if (MT->isLambdaToBlockConversionOperator())
        return;
      if (MT->isOutOfLine())
        return;
      if (MT->isStaticOverloadedOperator())
        return;
      if (MT->isTemplated())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT->isCopyConstructor())
        return;
      if (MT->isMoveAssignmentOperator())
        return;
      if (MT->isMoveConstructor())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->isStatic())
        return;
      if (MT->isUserProvided())
        return;
      if (MT->isVirtualAsWritten())
        return;
      if (MT->hasBody())
        return;
      if (MT->hasInlineBody())
        return;
      if (MT->hasInheritedPrototype())
        return;
      if (MT->hasOverloadedOperatorName())
        return;
      if (MT->hasPrototype())
        return;
      if (MT->hasWrittenPrototype())
        return;
      if (MT->isCopyAssignmentOperator())
        return;
      if (MT