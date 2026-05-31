//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"

/**
 * Modify_Return_Type_For_Recursive_Template_Instantiation_13
 */ 
class MutatorFrontendAction_Modify_Return_Type_For_Recursive_Template_Instantiation_13 : public MutatorFrontendAction {
public:
    std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(clang::CompilerInstance &CI, llvm::StringRef InFile) override {
        TheRewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
        return std::make_unique<MutatorASTConsumer_Modify_Return_Type_For_Recursive_Template_Instantiation_13>(TheRewriter);
    }

private:
    class MutatorASTConsumer_Modify_Return_Type_For_Recursive_Template_Instantiation_13 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Modify_Return_Type_For_Recursive_Template_Instantiation_13(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Modify_Return_Type_For_Recursive_Template_Instantiation_13.h"
#include "clang/ASTMatchers/ASTMatchers.h"

// ========================================================================================================
#define MUT_MODIFY_RETURN_TYPE_FOR_RECURSIVE_TEMPLATE_INSTANTIATION_13_OUTPUT 1

void MutatorFrontendAction_Modify_Return_Type_For_Recursive_Template_Instantiation_13::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("Function")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      // Replace the return type declaration with a version that references the template member function itself
      std::string mutatedDeclaration = declaration;
      size_t pos = mutatedDeclaration.find("->");
      if (pos != std::string::npos) {
        mutatedDeclaration.replace(pos + 2, mutatedDeclaration.find_first_of(" \n", pos + 2) - pos - 2, "decltype((*this)(arg))");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_Modify_Return_Type_For_Recursive_Template_Instantiation_13::MutatorASTConsumer_Modify_Return_Type_For_Recursive_Template_Instantiation_13::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasTemplateParameterList(isAnyValue())).bind("Function");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}