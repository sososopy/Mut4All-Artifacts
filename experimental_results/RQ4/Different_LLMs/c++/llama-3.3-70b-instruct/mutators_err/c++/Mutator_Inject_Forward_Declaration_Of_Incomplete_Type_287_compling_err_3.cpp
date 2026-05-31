//header file
#pragma once
#include "Mutator_base.h"

/**
 * Inject_Forward_Declaration_Of_Incomplete_Type_287
 */ 
class MutatorFrontendAction_287 : public clang::ASTFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override;

private:
    class MutatorASTConsumer_287 : public clang::ASTConsumer {
    public:
        MutatorASTConsumer_287(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(clang::ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public clang::ast_matchers::MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const clang::ast_matchers::MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Inject_Forward_Declaration_Of_Incomplete_Type_287.h"

// ========================================================================================================
#define MUT287_OUTPUT 1

void MutatorFrontendAction_287::Callback::run(const clang::ast_matchers::MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("RecordDecls")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = clang::Lexer::getSourceText(
          clang::CharSourceRange::getTokenRange(MT->getSourceRange()), 
          *Result.SourceManager, 
          clang::LangOptions());
      //Perform mutation on the source code text by applying string replacement
      std::string forward_declaration = "struct " + MT->getNameAsString() + ";\n";
      std::string mutated_declaration = declaration;
      size_t pos = mutated_declaration.find("{");
      if (pos != std::string::npos) {
        mutated_declaration.insert(pos + 1, forward_declaration);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(MT->getSourceRange()), mutated_declaration);
    }
}
  
std::unique_ptr<clang::ASTConsumer> MutatorFrontendAction_287::CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) {
    Rewriter TheRewriter(CI.getSourceManager(), CI.getLangOpts());
    TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
    return std::make_unique<MutatorASTConsumer_287>(TheRewriter);
}

void MutatorFrontendAction_287::MutatorASTConsumer_287::HandleTranslationUnit(clang::ASTContext &Context) {
    clang::ast_matchers::MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    clang::ast_matchers::DeclarationMatcher matcher = clang::ast_matchers::cxxRecordDecl().bind("RecordDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}