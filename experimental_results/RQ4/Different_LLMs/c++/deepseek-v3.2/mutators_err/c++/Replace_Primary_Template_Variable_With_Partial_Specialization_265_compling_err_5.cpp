//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Primary_Template_Variable_With_Partial_Specialization_265
 */ 
class MutatorFrontendAction_265 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(265)

private:
    class MutatorASTConsumer_265 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_265(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarTemplateDecl*> targetTemplates;
        std::vector<const clang::ClassTemplateDecl*> enclosingTemplates;
    };
};

//source file
#include "../include/Mutator_Replace_Primary_Template_Variable_With_Partial_Specialization_265.h"

// ========================================================================================================
#define MUT265_OUTPUT 1

void MutatorFrontendAction_265::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VT = Result.Nodes.getNodeAs<clang::VarTemplateDecl>("VarTemplate")) {
      //Filter nodes in header files
      if (!VT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VT->getLocation()))
        return;
      //Get the source code text of target node
      auto* VD = VT->getTemplatedDecl();
      if (!VD || !VD->isStaticDataMember())
        return;
      if (!VD->getType()->isBooleanType())
        return;
      //Record the node information to be used in the mutation process
      targetTemplates.push_back(VT);
      //Find enclosing template struct
      auto* RD = dyn_cast<clang::CXXRecordDecl>(VD->getDeclContext());
      if (!RD) return;
      auto* CTD = RD->getDescribedClassTemplate();
      if (!CTD) return;
      enclosingTemplates.push_back(CTD);
    }
    else if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplate")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      //Find matching var template inside this class template
      for (size_t i = 0; i < enclosingTemplates.size(); ++i) {
        if (enclosingTemplates[i] == CT && i < targetTemplates.size()) {
          auto* VT = targetTemplates[i];
          auto* VD = VT->getTemplatedDecl();
          auto* RD = CT->getTemplatedDecl();
          //Get the source code text of target node
          auto varDeclText = stringutils::rangetoStr(*(Result.SourceManager), VD->getSourceRange());
          auto classTemplateText = stringutils::rangetoStr(*(Result.SourceManager), RD->getSourceRange());
          //Extract template parameters from class template
          auto* TP =1 CT->getTemplateParameters();
          std::string templateParamsStr;
          if (TP && TP->size() > 0) {
            templateParamsStr = stringutils::rangetoStr(*(Result.SourceManager), TP->getSourceRange());
          }
          //Perform mutation on the source code text by applying string replacement
          //Build partial specialization
          std::string partialSpec = "\n\n  template <template <" + templateParamsStr + "> class C, " + templateParamsStr + "... Args>\n";
          partialSpec += "  static constexpr bool value<C<Args...>, C> = true;\n";
          //Insert partial specialization before the closing brace of the class
          auto classEndLoc = RD->getEndLoc();
          //Replace the original AST node with the mutated one
          Rewrite.InsertTextBefore(classEndLoc, partialSpec);
          //Remove this pair from vectors to avoid duplicate mutations
          targetTemplates.erase(targetTemplates.begin() + i);
          enclosingTemplates.erase(enclosingTemplates.begin() + i);
          break;
        }
      }
    }
}
  
void MutatorFrontendAction_265::MutatorASTConsumer_265::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher varMatcher = varTemplateDecl(hasAncestor(classTemplateDecl())).bind("VarTemplate");
    DeclarationMatcher classMatcher = classTemplateDecl().bind("ClassTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(classMatcher, &callback);
    matchFinder.matchAST(Context);
}