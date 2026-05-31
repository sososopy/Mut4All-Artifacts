//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Pseudo_Destructor_Type_211
 */ 
class MutatorFrontendAction_211 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(211)

private:
    class MutatorASTConsumer_211 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_211(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::Type *> types;
    };
};

//source file
#include "../include/Replace_Pseudo_Destructor_Type_211.h"

// ========================================================================================================
#define MUT211_OUTPUT 1

void MutatorFrontendAction_211::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::UnaryExprOrTypeTraitExpr>("PseudoDestructor")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto pseudoDestructor = MT->getType();
      if (auto *packExpansion = dyn_cast<PackExpansionType>(pseudoDestructor)) {
        auto *patternType = packExpansion->getPattern();
        if (auto *type = dyn_cast TypedefType>(patternType)) {
          auto *decl = type->getDecl();
          if (auto *templateParam = dyn_cast<TemplateTypeParmType>(decl->getType())) {
            //Perform mutation on the source code text by applying string replacement
            std::string replacementType;
            if (!types.empty()) {
              replacementType = types[0]->getAsString();
            } else {
              replacementType = "int";
            }
            std::string mutatedCode = "(*p).~" + replacementType + "...[]();";
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedCode);
          }
        }
      }
    } else if (auto *TD = Result.Nodes.getNodeAs<clang::Type>("Type")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      types.push_back(TD);
    }
}
  
void MutatorFrontendAction_211::MutatorASTConsumer_211::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto pseudoDestructorMatcher = unaryExprOrTypeTraitExpr(hasType(packExpansionType())).bind("PseudoDestructor");
    auto typeMatcher = type().bind("Type");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(pseudoDestructorMatcher, &callback);
    matchFinder.addMatcher(typeMatcher, &callback);
    matchFinder.matchAST(Context);
}