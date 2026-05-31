//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Template_Adaptors_54
 */ 
class MutatorFrontendAction_54 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(54)

private:
    class MutatorASTConsumer_54 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_54(Rewriter &R) : TheRewriter(R) {}
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
        bool adaptorsInserted = false;
    };
};

//source file
#include "../include/Nested_Template_Adaptors_54.h"

// ========================================================================================================
#define MUT54_OUTPUT 1

void MutatorFrontendAction_54::Callback::run(const MatchFinder::MatchResult &Result) {
    SourceManager &SM = Rewrite.getSourceMgr();
    if (auto *expr = Result.Nodes.getNodeAs<clang::Expr>("senderExpr")) {
        if (!expr || !Result.Context->getSourceManager().isWrittenInMainFile(expr->getBeginLoc()))
            return;

        if (!adaptorsInserted) {
            SourceLocation startLoc = SM.getLocForStartOfFile(SM.getMainFileID());
            std::string adaptorCode = R"(
template <template <typename> class T, typename U>
struct Adapt1 {
    using type = T<U>;
};

template <template <typename> class T, typename U>
struct Adapt2 {
    using type = T<U>;
};

template <template <typename> class T, typename U>
struct Adapt3 {
    using type = T<U>;
};
)";
            Rewrite.InsertText(startLoc, adaptorCode, true, true);
            adaptorsInserted = true;
        }

        std::string originalCode = stringutils::rangetoStr(*Result.SourceManager, expr->getSourceRange());
        std::string adaptedCode = "Adapt1<Adapt2<Adapt3<", adaptedTail = ">>>::type";
        adaptedCode += originalCode + ">"; 
        adaptedCode += adaptedTail;
        Rewrite.ReplaceText(expr->getSourceRange(), adaptedCode);
    }
}
  
void MutatorFrontendAction_54::MutatorASTConsumer_54::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = declRefExpr(to(varDecl())).bind("senderExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}