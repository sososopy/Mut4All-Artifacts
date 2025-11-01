//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Default_Template_Argument_Attribute_1
 */ 
class MutatorFrontendAction_1 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(1)

private:
    class MutatorASTConsumer_1 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_1(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/add_default_template_argument_attribute_1.h"

// ========================================================================================================
#define MUT1_OUTPUT 1

void MutatorFrontendAction_1::Callback::run(const MatchFinder::MatchResult &Result) {
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
      size_t pos = declaration.find("template");
      if (pos != std::string::npos) {
          pos = declaration.find("{", pos);
          if (pos != std::string::npos) {
              declaration.insert(pos, "\n[[nodiscard]] ");
          }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FTD->getSourceRange()), declaration);
    }
}

void MutatorFrontendAction_1::MutatorASTConsumer_1::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(
        hasAnyTemplateParameter(templateTypeParmDecl(hasDefaultArgument()))).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}