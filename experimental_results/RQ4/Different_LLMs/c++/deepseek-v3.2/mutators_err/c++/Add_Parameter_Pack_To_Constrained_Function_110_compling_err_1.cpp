//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Parameter_Pack_To_Constrained_Function_110
 */ 
class MutatorFrontendAction_110 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(110)

private:
    class MutatorASTConsumer_110 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_110(Rewriter &R) : TheRewriter(R) {}
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
        std::string conceptName;
    };
};

//source file
#include "../include/Mutator_Add_Parameter_Pack_To_Constrained_Function_110.h"

// ========================================================================================================
#define MUT110_OUTPUT 1

void MutatorFrontendAction_110::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("funcDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (!FD->isFunctionTemplateSpecialization() && FD->getDescribedFunctionTemplate()) {
        auto *FT = FD->getDescribedFunctionTemplate();
        if (FT->getTemplateParameters()->size() == 1) {
          auto *TP = *FT->getTemplateParameters()->begin();
          if (TP->isTemplateParameterPack()) return;
          if (FT->hasAssociatedConstraints()) {
            for (auto *C : FT->getAssociatedConstraints()) {
              if (auto *RE = C->getConstraintExpr()) {
                if (auto *CExpr = dyn_cast<ConceptSpecializationExpr>(RE)) {
                  conceptName = CExpr->getNamedConcept()->getNameAsString();
                  //Get the source code text of target node
                  auto funcText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());
                  auto templateText = stringutils::rangetoStr(*(Result.SourceManager), FT->getSourceRange());
                  //Perform mutation on the source code text by applying string replacement
                  size_t templateEnd = templateText.find('>');
                  if (templateEnd != std::string::npos) {
                    templateText.insert(templateEnd, ", typename... Args");
                  }
                  size_t requiresPos = funcText.find("requires");
                  if (requiresPos != std::string::npos) {
                    size_t requiresEnd = funcText.find('{', requiresPos);
                    if (requiresEnd != std::string::npos) {
                      std::string newConstraint = "requires " + conceptName + "<T> && (" + conceptName + "<Args> && ...) ";
                      funcText.replace(requiresPos, requiresEnd - requiresPos, newConstraint);
                    }
                  }
                  size_t bodyStart = funcText.find('{');
                  if (bodyStart != std::string::npos) {
                    funcText.insert(bodyStart + 1, "\n    static_assert(sizeof...(Args) >= 0);\n");
                  }
                  std::string mutatedText = templateText + "\n" + funcText;
                  //Replace the original AST node with the mutated one
                  Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), mutatedText);
                }
              }
            }
          }
        }
      }
    }
}
  
void MutatorFrontendAction_110::MutatorASTConsumer_110::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAssociatedConstraints(), isTemplateInstantiation()).bind("funcDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}