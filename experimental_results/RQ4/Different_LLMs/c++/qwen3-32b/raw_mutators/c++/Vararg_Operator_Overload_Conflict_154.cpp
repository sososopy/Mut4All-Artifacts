//header file
#pragma once
#include "Mutator_base.h"

/**
 * Vararg_Operator_Overload_Conflict_154
 */ 
class MutatorFrontendAction_154 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(154)

private:
    class MutatorASTConsumer_154 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_154(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Vararg_Operator_Overload_Conflict_154.h"

// ========================================================================================================
#define MUT154_OUTPUT 1

void MutatorFrontendAction_154::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassWithOperator")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        bool hasFriendOperator = false;
        for (const auto *FD : MT->friends()) {
            if (FD->getFriendDecl()->isFunction()) {
                const FunctionDecl *FDFunc = FD->getFriendDecl()->getAsFunction<FunctionDecl>();
                if (FDFunc && FDFunc->getName().startswith("operator")) {
                    hasFriendOperator = true;
                    break;
                }
            }
        }
        if (!hasFriendOperator)
            return;

        SourceManager &SM = Result.Context->getSourceManager();
        SourceLocation startLoc = MT->getBeginLoc();
        SourceLocation endLoc = MT->getEndLoc();
        if (SM.isInvalidLoc(startLoc) || SM.isInvalidLoc(endLoc))
            return;

        std::string classText = stringutils::rangetoStr(SM, CharSourceRange::getTokenRange(MT->getSourceRange()));

        std::string className = MT->getNameAsString();
        std::string newOperators = 
            "\npublic:\n"
            "  friend void operator,(" + className + ", int);\n"
            "private:\n"
            "  friend void operator,(" + className + ", ...);\n";

        size_t closeBracePos = classText.rfind('}');
        if (closeBracePos == std::string::npos)
            return;

        std::string mutatedText = classText.substr(0, closeBracePos) + newOperators + classText.substr(closeBracePos);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedText);
    }
}

void MutatorFrontendAction_154::MutatorASTConsumer_154::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(
        hasFriendDecl(decl(isFunction(), hasName(starts_with("operator")))))
        .bind("ClassWithOperator");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}