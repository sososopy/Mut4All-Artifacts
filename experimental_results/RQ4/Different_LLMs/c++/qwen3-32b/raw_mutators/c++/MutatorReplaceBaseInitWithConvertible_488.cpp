//header file
#pragma once
#include "Mutator_base.h"

/**
 * MutatorReplaceBaseInitWithConvertible_488
 */ 
class MutatorFrontendAction_488 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(488)

private:
    class MutatorASTConsumer_488 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_488(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite), helperCounter(0) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        int helperCounter;
    };
};

//source file
#include "../include/MutatorReplaceBaseInitWithConvertible_488.h"

// ========================================================================================================
#define MUT488_OUTPUT 1

void MutatorFrontendAction_488::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *ctor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("ctor")) {
        if (!ctor || !Result.Context->getSourceManager().isWrittenInMainFile(ctor->getLocation()))
            return;

        if (auto *baseInitExpr = Result.Nodes.getNodeAs<clang::CXXConstructExpr>("baseInitExpr")) {
            QualType baseType = baseInitExpr->getConstructedType();
            std::string baseTypeName = baseType.getAsString();

            std::string helperName = "Helper_" + std::to_string(helperCounter);
            std::string helperInstanceName = "h_" + std::to_string(helperCounter);
            ++helperCounter;

            std::string helperCode = "class " + helperName + " {\npublic:\n    operator " + baseTypeName + "() const;\n};\n";
            helperCode += helperInstanceName + ";\n";

            auto *derivedClass = ctor->getParent();
            SourceLocation insertLoc = derivedClass->getBeginLoc();
            if (insertLoc.isInvalid())
                return;

            Rewrite.InsertTextBefore(insertLoc, helperCode);

            SourceRange exprRange(baseInitExpr->getBeginLoc(), baseInitExpr->getEndLoc());
            Rewrite.ReplaceText(exprRange, helperInstanceName);
        }
    }
}
  
void MutatorFrontendAction_488::MutatorASTConsumer_488::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxConstructorDecl(
        hasAnyConstructorInitializer(
            cxxBaseInitializer(has(cxxConstructExpr().bind("baseInitExpr")))
        )
    ).bind("ctor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}