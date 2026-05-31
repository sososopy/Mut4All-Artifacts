//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateInstantiationWithDependentBases_274
 */ 
class MutatorFrontendAction_274 : public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override;

private:
    class MutatorASTConsumer_274 : public clang::ASTConsumer {
    public:
        MutatorASTConsumer_274(clang::Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(clang::ASTContext &Context) override;
    private:
        clang::Rewriter &TheRewriter;
    
    };
    
    class Callback : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
        Callback(clang::Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result);
    private:
        clang::Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_TemplateInstantiationWithDependentBases_274.h"

// ========================================================================================================
#define MUT274_OUTPUT 1

void MutatorFrontendAction_274::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          Result.Context->getLangOpts());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedCode = "template <typename T> class MyNewClass : public T::Base { };";
      mutatedCode += "\ntemplate <typename T> void myFunction() { MyNewClass<T> obj; obj.baseMember(); }";
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedCode);
    }
}
  
void MutatorFrontendAction_274::MutatorASTConsumer_274::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    clang::ast_matchers::DeclarationMatcher matcher = clang::ast_matchers::classTemplateDecl().bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}

std::unique_ptr<clang::ASTConsumer> MutatorFrontendAction_274::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) {
    return std::make_unique<MutatorASTConsumer_274>(clang::Rewriter(CI.getSourceManager(), CI.getLangOpts()));
}