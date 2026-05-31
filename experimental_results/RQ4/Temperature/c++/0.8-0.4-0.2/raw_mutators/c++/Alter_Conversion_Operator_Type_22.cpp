//header file
#pragma once
#include "Mutator_base.h"

/**
 * Alter_Conversion_Operator_Type_22
 */ 
class MutatorFrontendAction_22 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(22)

private:
    class MutatorASTConsumer_22 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_22(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/alter_conversion_operator_type_22.h"

// ========================================================================================================
#define MUT22_OUTPUT 1

void MutatorFrontendAction_22::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXConversionDecl>("ConversionOp")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string newType = "int"; // Example of a standard type replacement
      size_t pos = declaration.find("operator");
      if (pos != std::string::npos) {
          pos = declaration.find('(', pos);
          if (pos != std::string::npos) {
              declaration.replace(declaration.find(')', pos), 1, newType + ")");
          }
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_22::MutatorASTConsumer_22::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConversionDecl().bind("ConversionOp");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}