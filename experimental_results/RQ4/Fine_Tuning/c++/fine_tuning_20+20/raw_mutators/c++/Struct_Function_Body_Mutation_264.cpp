//header file
#pragma once
#include "Mutator_base.h"

/**
 * Struct_Function_Body_Mutation_264
 */ 
class MutatorFrontendAction_264 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(264)

private:
    class MutatorASTConsumer_264 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_264(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/struct_function_body_mutation_264.h"

// ========================================================================================================
#define MUT264_OUTPUT 1

void MutatorFrontendAction_264::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;

      if (MT->hasBody()) {
        auto body = MT->getBody();
        auto body_str = stringutils::rangetoStr(*(Result.SourceManager),
                                                body->getSourceRange());
        llvm::outs() << body_str << '\n';
        int dice = getrandom::getRandomIndex(2);
        if (dice == 0) {
          if (body_str.rfind('}') != string::npos)
            body_str.replace(body_str.rfind('}'), 1, "]");
        } else if (dice == 1) {
          if (body_str.rfind('}') != string::npos)
            body_str.erase(body_str.rfind('}'), 1);
        } else if (dice == 2) {
          if (body_str.rfind('}') != string::npos)
            body_str += "}";
        }
        body_str = "/*mut264*/" + body_str;
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(body->getSourceRange()), body_str);
      }
    }
}
  
void MutatorFrontendAction_264::MutatorASTConsumer_264::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl().bind("Methods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}