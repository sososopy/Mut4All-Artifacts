//header file
#pragma once
#include "Mutator_base.h"

/**
 * Transform_Function_To_Lambda_With_Requires_Clause_482
 */ 
class MutatorFrontendAction_482 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(482)

private:
    class MutatorASTConsumer_482 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_482(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Transform_Function_To_Lambda_With_Requires_Clause_482.h"

// ========================================================================================================
#define MUT482_OUTPUT 1

void MutatorFrontendAction_482::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      llvm::outs() << declaration << '\n';
      if (MT->isFunctionTemplateSpecialization())
        return;
      if (MT->isTemplated()) {
        if (MT->getTemplateSpecializationKind() !=
            TemplateSpecializationKind::TSK_Undeclared)
          return;
      }
      if (MT->isTemplateInstantiation())
        return;
      if (MT->isDefaulted())
        return;
      if (MT->isDeleted())
        return;
      if (MT->isMain())
        return;
      if (MT->isOverloadedOperator())
        return;
      if (MT->getTemplatedKind() != FunctionDecl::TemplatedKind::TK_NonTemplate)
        return;
      if (MT->isInExternCContext())
        return;
      if (MT->isInExternCXXContext())
        return;
      if (MT->isInAnonymousNamespace())
        return;
      if (MT->isInStdNamespace())
        return;
      if (MT->isInGlobalNamespace())
        return;
      if (MT->isInLocalScope())
        return;
      if (MT->isInlined())
        return;
      if (MT->isVariadic())
        return;
      if (MT->isVirtual())
        return;
      if (MT->isPure())
        return;
      if (MT->isTrivial())
        return;
      if (MT->isTriviallyCopyable())
        return;
      if (MT->isTriviallyDefaultConstructible())
        return;
      if (MT->isTriviallyCopyConstructible())
        return;
      if (MT->isTriviallyMoveConstructible())
        return;
      if (MT->isTriviallyCopyAssignable())
        return;
      if (MT->isTriviallyMoveAssignable())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriviallyDestructible())
        return;
      if (MT->isTriviallyConstructible())
        return;
      if (MT->isTriviallyAssignable())
        return;
      if (MT->isTriviallyEqualityComparable())
        return;
      if (MT->isTriviallyComparable())
        return;
      if (MT->isTriviallySwappable())
        return;
      if (MT->isTriviallySwappableWith())
        return;
      if (MT->isTriv