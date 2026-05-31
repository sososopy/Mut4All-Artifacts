//header file
#pragma once
#include "Mutator_base.h"

/**
 * TemplateInstantiationWithIncompatibleTypes_153
 */ 
class MutatorFrontendAction_TemplateInstantiationWithIncompatibleTypes_153 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(TemplateInstantiationWithIncompatibleTypes_153)

private:
    class MutatorASTConsumer_TemplateInstantiationWithIncompatibleTypes_153 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_TemplateInstantiationWithIncompatibleTypes_153(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_TemplateInstantiationWithIncompatibleTypes_153.h"

// ========================================================================================================
#define MUTTEMPLATEINSTANTIATIONWITHINCOMPATIBLETYPES_153_OUTPUT 1

void MutatorFrontendAction_TemplateInstantiationWithIncompatibleTypes_153::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CallExpr>("CallExpr")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      if (MT->getNumArgs() > 0) {
        auto arg = MT->getArg(0);
        if (auto *T = dyn_cast<clang::Expr>(arg)) {
          if (auto *TE = dyn_cast<clang::TemplateArgument>(T)) {
            if (TE->getKind() == clang::TemplateArgument::Type) {
              auto type = TE->getAsType();
              if (type->isPointerType()) {
                // Change pointer type to reference type
                declaration.replace(declaration.find('*'), 1, "&");
              } else if (type->isReferenceType()) {
                // Change reference type to pointer type
                declaration.replace(declaration.find('&'), 1, "*");
              }
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_TemplateInstantiationWithIncompatibleTypes_153::MutatorASTConsumer_TemplateInstantiationWithIncompatibleTypes_153::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = callExpr(hasArgument(0, hasType(pointerType())));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}