//header file
#pragma once
#include "Mutator_base.h"

/**
 * Specialize_Member_Function_Template_With_Local_Type_398
 */ 
class MutatorFrontendAction_398 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(398)

private:
    class MutatorASTConsumer_398 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_398(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Specialize_Member_Function_Template_With_Local_Type_398.h"

// ========================================================================================================
#define MUT398_OUTPUT 1

void MutatorFrontendAction_398::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("MethodTemplates")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      if (content.find("[]") == string::npos)
        return;
      llvm::outs() << content << '\n';
      //Perform mutation on the source code text by applying string replacement
      if (content.find("[]") != string::npos) {
        auto lambda =
            stringutils::get_lambda(content, content.find("[]") + 1);
        llvm::outs() << lambda << '\n';
        auto local_type = stringutils::lambda2local(lambda);
        content.replace(content.find(lambda), lambda.size(), local_type);
        llvm::outs() << content << '\n';
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_398::MutatorASTConsumer_398::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(hasDescendant(lambdaExpr())).bind("MethodTemplates");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}