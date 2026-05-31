//header file
#pragma once
#include "Mutator_base.h"

/**
 * Recursive_Template_Instantiation_With_Lambda_In_Return_Type_Deduction_Context_267
 */ 
class MutatorFrontendAction_267 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(267)

private:
    class MutatorASTConsumer_267 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_267(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::FunctionTemplateDecl*> templateDecls;
    };
};

//source file
#include "../include/Mutator_Recursive_Template_Instantiation_With_Lambda_In_Return_Type_Deduction_Context_267.h"

// ========================================================================================================
#define MUT267_OUTPUT 1

void MutatorFrontendAction_267::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateFunc")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      templateDecls.push_back(MT);
    }
    else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FuncBody")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Get the source code text of target node
      if (!FD->hasBody())
        return;
      //Check if this function belongs to one of the recorded templates
      const clang::FunctionTemplateDecl* parentTemplate = nullptr;
      for (auto tmpl : templateDecls) {
        if (tmpl->getTemplatedDecl() == FD) {
          parentTemplate = tmpl;
          break;
        }
      }
      if (!parentTemplate)
        return;
      //Perform mutation on the source code text by applying string replacement
      std::string insertion = "\n/*mut267*/auto recursive_var = " + FD->getNameAsString() + "([]{});\n";
      //Insert at the beginning of the function body
      clang::CompoundStmt* body = llvm::dyn_cast<clang::CompoundStmt>(FD->getBody());
      if (body && body->size() > 0) {
        clang::Stmt* firstStmt = *(body->body_begin());
        clang::SourceLocation insertLoc = firstStmt->getBeginLoc();
        //Replace the original AST node with the mutated one
        Rewrite.InsertTextBefore(insertLoc, insertion);
      } else {
        //Insert right after the opening brace
        clang::SourceRange bodyRange = FD->getBody()->getSourceRange();
        clang::SourceLocation insertLoc = bodyRange.getBegin().getLocWithOffset(1);
        Rewrite.InsertTextAfterToken(insertLoc, insertion);
      }
    }
}
  
void MutatorFrontendAction_267::MutatorASTConsumer_267::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher templateMatcher = functionTemplateDecl().bind("TemplateFunc");
    DeclarationMatcher funcMatcher = functionDecl(hasBody(compoundStmt())).bind("FuncBody");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
}