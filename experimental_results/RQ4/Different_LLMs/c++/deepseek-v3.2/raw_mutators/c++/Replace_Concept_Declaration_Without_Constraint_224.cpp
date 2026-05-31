//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Concept_Declaration_Without_Constraint_224
 */ 
class MutatorFrontendAction_224 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(224)

private:
    class MutatorASTConsumer_224 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_224(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarDecl*> availableVariables;
        std::vector<const clang::TypeDecl*> availableTypes;
    };
};

//source file
#include "../include/Mutator_Replace_Concept_Declaration_Without_Constraint_224.h"

// ========================================================================================================
#define MUT224_OUTPUT 1

void MutatorFrontendAction_224::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ConceptDecl>("ConceptDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Check if concept has constraint expression
      if (MT->getConstraintExpr())
        return;
      //Get the source code text of target node
      auto conceptText = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Construct constraint expression
      std::string constraintExpr;
      //Try to use existing template parameters
      auto templateParams = MT->getTemplateParameters();
      if (templateParams && templateParams->size() > 0) {
        auto firstParam = templateParams->getParam(0);
        if (firstParam->isTemplateTypeParam()) {
          constraintExpr = "requires { typename " + firstParam->getNameAsString() + "; }";
        } else if (firstParam->isNonTypeTemplateParam()) {
          constraintExpr = "requires { " + firstParam->getNameAsString() + "; }";
        }
      } else {
        //Use available variables or types
        if (!availableVariables.empty()) {
          auto var = availableVariables[getrandom::getRandomIndex(availableVariables.size()-1)];
          constraintExpr = "requires { " + var->getNameAsString() + "; }";
        } else if (!availableTypes.empty()) {
          auto type = availableTypes[getrandom::getRandomIndex(availableTypes.size()-1)];
          constraintExpr = "requires { typename " + type->getNameAsString() + "; }";
        } else {
          //Add dummy declaration and constraint
          constraintExpr = "requires { int dummy = 0; dummy; }";
        }
      }
      //Perform mutation on the source code text by applying string replacement
      size_t pos = conceptText.find(";");
      if (pos != std::string::npos) {
        conceptText.insert(pos, " = " + constraintExpr);
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), conceptText);
    }
    //Collect available variables and types
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      availableVariables.push_back(VD);
    }
    else if (auto *TD = Result.Nodes.getNodeAs<clang::TypeDecl>("TypeDecl")) {
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      availableTypes.push_back(TD);
    }
}
  
void MutatorFrontendAction_224::MutatorASTConsumer_224::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher conceptMatcher = conceptDecl().bind("ConceptDecl");
    DeclarationMatcher varMatcher = varDecl().bind("VarDecl");
    DeclarationMatcher typeMatcher = typeDecl().bind("TypeDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(conceptMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.matchAST(Context);
}