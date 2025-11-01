//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_CRTP_With_Consteval_And_Static_Cast_294
 */ 
class MutatorFrontendAction_294 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(294)

private:
    class MutatorASTConsumer_294 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_294(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Modify_CRTP_With_Consteval_And_Static_Cast_294.h"

// ========================================================================================================
#define MUT294_OUTPUT 1

void MutatorFrontendAction_294::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CXXStaticCastExpr>("StaticCastExpr")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getBeginLoc()))
        return;
      
      //Get the source code text of target node
      auto castText = stringutils::rangetoStr(*(Result.SourceManager), CE->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      if (castText.find("static_cast<") != std::string::npos) {
          castText = "static_cast<Derived&>(*this)";
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CE->getSourceRange()), castText);
    }
}
  
void MutatorFrontendAction_294::MutatorASTConsumer_294::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = cxxStaticCastExpr(
        hasAncestor(cxxMethodDecl(isConstexpr(), ofClass(classTemplateSpecializationDecl(hasName("Base")))))
    ).bind("StaticCastExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}