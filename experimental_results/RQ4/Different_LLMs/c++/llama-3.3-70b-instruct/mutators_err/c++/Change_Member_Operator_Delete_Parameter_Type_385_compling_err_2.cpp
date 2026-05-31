//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Member_Operator_Delete_Parameter_Type_385
 */ 
class MutatorFrontendAction_385 : public MutatorFrontendAction {
public:
    std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_385>(TheRewriter);
    }

private:
    class MutatorASTConsumer_385 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_385(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Change_Member_Operator_Delete_Parameter_Type_385.h"

// ========================================================================================================
#define MUT385_OUTPUT 1

void MutatorFrontendAction_385::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("MemberOperatorDelete")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      // Check if the method is a member operator delete
      if (!MT->isUserProvided() || !MT->isMemberFunction() || !MT->getNameAsString().find("operator delete"))
        return;
      // Get the first parameter of the member operator delete
      auto params = MT->parameters();
      if (params.size() < 1)
        return;
      auto param = params[0];
      // Check if the first parameter is not a class type
      if (param->getType()->isClassType())
        return;
      // Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      // Perform mutation on the source code text by applying string replacement
      // Replace the type of the first parameter with a class type
      std::string newType = "struct NewType";
      size_t pos = declaration.find(param->getType().getAsString());
      declaration.replace(pos, param->getType().getAsString().size(), newType);
      // Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_385::MutatorASTConsumer_385::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxMethodDecl(hasName("operator delete"), isMemberFunction()).bind("MemberOperatorDelete");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}