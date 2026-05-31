//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Template_Argument_In_Constexpr_Static_Member_49
 */ 
class MutatorFrontendAction_49 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(49)

private:
    class MutatorASTConsumer_49 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_49(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
        std::vector<const clang::VarDecl*> candidateDecls;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Mutator_Replace_Template_Argument_In_Constexpr_Static_Member_49.h"

// ========================================================================================================
#define MUT49_OUTPUT 1

void MutatorFrontendAction_49::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("staticMember")) {
      //Filter nodes in header files
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Check if it's a constexpr static member
      if (!VD->isStaticDataMember() || !VD->isConstexpr())
        return;
      //Get the source code text of target node
      auto init = VD->getInit();
      if (!init)
        return;
      //Check if initialization is a call expression with explicit template arguments
      auto *callExpr = llvm::dyn_cast<clang::CallExpr>(init);
      if (!callExpr)
        return;
      auto *callee = callExpr->getCallee();
      if (!callee)
        return;
      auto *declRefExpr = llvm::dyn_cast<clang::DeclRefExpr>(callee);
      if (!declRefExpr)
        return;
      auto *funcDecl = llvm::dyn_cast<clang::FunctionDecl>(declRefExpr->getDecl());
      if (!funcDecl || !funcDecl->isTemplateInstantiation())
        return;
      //Check if the call has explicit template arguments
      auto *templateArgs = declRefExpr->getTemplateArgs();
      if (!templateArgs)
        return;
      //Record the node information to be used in the mutation process
      candidateDecls.push_back(VD);
    }
    else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("selectedMember")) {
      if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     VD->getLocation()))
        return;
      //Find this decl in candidateDecls
      auto it = std::find(candidateDecls.begin(), candidateDecls.end(), VD);
      if (it == candidateDecls.end())
        return;
      //Get the source code text of target node
      auto init = VD->getInit();
      auto *callExpr = llvm::dyn_cast<clang::CallExpr>(init);
      auto *declRefExpr = llvm::dyn_cast<clang::DeclRefExpr>(callExpr->getCallee());
      auto *templateArgs = declRefExpr->getTemplateArgs();
      //Get surrounding context to find dependent types
      auto *parent = VD->getDeclContext();
      std::string extraArg = "";
      if (auto *recordDecl = llvm::dyn_cast<clang::CXXRecordDecl>(parent)) {
        if (recordDecl->getTemplateInstantiationPattern()) {
          extraArg = "T";
        } else {
          //Look for template parameters in enclosing scopes
          auto *tplDecl = recordDecl->getDescribedClassTemplate();
          if (tplDecl) {
            auto *tplParams = tplDecl->getTemplateParameters();
            if (tplParams && tplParams->size() > 0) {
              extraArg = tplParams->getParam(0)->getNameAsString();
            }
          }
        }
      }
      if (extraArg.empty()) {
        extraArg = "0"; //Use integer literal as non-type template argument
      }
      //Perform mutation on the source code text by applying string replacement
      std::string newArgs = "<";
      if (templateArgs) {
        unsigned numArgs = templateArgs->size();
        for (unsigned i = 0; i < numArgs; ++i) {
          if (i > 0) newArgs += ", ";
          newArgs += templateArgs->getArg(i).getAsString();
        }
        newArgs += ", " + extraArg + ">";
        //Replace the original template argument list
        auto langOpts = Rewrite.getLangOpts();
        auto templateArgRange = declRefExpr->getSourceRange();
        if (templateArgRange.isValid()) {
          //Replace the original AST node with the mutated one
          Rewrite.ReplaceText(templateArgRange, newArgs);
        }
      }
    }
}
  
void MutatorFrontendAction_49::MutatorASTConsumer_49::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl(isStaticStorageClass(), isConstexpr(), hasInitializer(callExpr().bind("callExpr"))).bind("staticMember");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
    //Select one candidate for mutation
    if (!callback.candidateDecls.empty()) {
      MatchFinder secondFinder;
      size_t idx = getrandom::getRandomIndex(callback.candidateDecls.size() - 1);
      auto *selectedDecl = callback.candidateDecls[idx];
      DeclarationMatcher selectMatcher = varDecl(hasDescendant(declRefExpr(to(equalsNode(selectedDecl))))).bind("selectedMember");
      secondFinder.addMatcher(selectMatcher, &callback);
      secondFinder.matchAST(Context);
    }
}