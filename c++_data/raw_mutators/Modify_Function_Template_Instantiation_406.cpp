//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Function_Template_Instantiation_406
 */ 
class MutatorFrontendAction_406 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(406)

private:
    class MutatorASTConsumer_406 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_406(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_function_template_instantiation_406.h"

// ========================================================================================================
#define MUT406_OUTPUT 1

void MutatorFrontendAction_406::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
      //Filter nodes in header files
      if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FTD->getLocation()))
        return;

      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FTD->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string alias = "\nclass Wrapper { public: using ValueType = T; };\n";
      std::string modifiedFunction = "typename Wrapper<T>::ValueType result = a + b; return result;";
      size_t pos = declaration.find("{");
      if (pos != std::string::npos) {
          declaration.insert(pos + 1, alias);
          size_t retPos = declaration.find("return");
          if (retPos != std::string::npos) {
              declaration.replace(retPos, std::string("return a + b;").length(), modifiedFunction);
          }
      }

      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FTD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_406::MutatorASTConsumer_406::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}