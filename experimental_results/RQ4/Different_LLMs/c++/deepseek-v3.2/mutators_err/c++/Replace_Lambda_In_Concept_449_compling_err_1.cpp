//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_In_Concept_449
 */ 
class MutatorFrontendAction_449 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(449)

private:
    class MutatorASTConsumer_449 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_449(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> existingNames;
    };
};

//source file
#include "../include/Mutator_Replace_Lambda_In_Concept_449.h"

// ========================================================================================================
#define MUT449_OUTPUT 1

void MutatorFrontendAction_449::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *LE = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambdaExpr")) {
      //Filter nodes in header files
      if (!LE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LE->getLocation()))
        return;

      //Get the parent template argument
      auto *TA = Result.Nodes.getNodeAs<clang::TemplateArgument>("templateArg");
      if (!TA) return;

      //Check if the lambda is used as a template argument to a concept
      auto *ConceptRef = Result.Nodes.getNodeAs<clang::ConceptDecl>("conceptDecl");
      if (!ConceptRef) return;

      //Get the source code text of target node
      std::string lambdaText = stringutils::rangetoStr(*Result.SourceManager, LE->getSourceRange());

      //Generate a unique variable name
      std::string varName = "mutated_lambda_var_449";
      int suffix = 0;
      while (existingNames.find(varName + std::to_string(suffix)) != existingNames.end()) {
          suffix++;
      }
      varName = varName + std::to_string(suffix);
      existingNames.insert(varName);

      //Construct the variable declaration
      std::string varDecl = "inline constexpr auto " + varName + " = " + lambdaText + ";";

      //Find appropriate insertion location (before the concept usage)
      SourceLocation insertLoc = LE->getBeginLoc();
      //Walk up to find the nearest declaration context before the lambda
      auto *DC = Result.Context->getDeclContext(LE);
      if (DC) {
          //Insert at the beginning of the current scope
          if (auto *Decl = dyn_cast<Decl>(DC)) {
              insertLoc = Decl->getBeginLoc();
          }
      }

      //Insert the variable declaration
      Rewrite.InsertText(insertLoc, varDecl + "\n");

      //Replace the lambda expression with the variable name
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(LE->getSourceRange()), varName);
    }
}
  
void MutatorFrontendAction_449::MutatorASTConsumer_449::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = lambdaExpr(
        hasParent(
            templateArgument(
                hasParent(
                    conceptDecl().bind("conceptDecl")
                )
            ).bind("templateArg")
        )
    ).bind("lambdaExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}