//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Incomplete_Dependent_Template_Member_Access_293
 */ 
class MutatorFrontendAction_293 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(293)

private:
    class MutatorASTConsumer_293 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_293(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl*> outerTemplateDecls;
        std::vector<const clang::ClassTemplateDecl*> nestedTemplateDecls;
    };
};

//source file
#include "../include/Mutator_Insert_Incomplete_Dependent_Template_Member_Access_293.h"

// ========================================================================================================
#define MUT293_OUTPUT 1

void MutatorFrontendAction_293::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("OuterTemplate")) {
      //Filter nodes in header files
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      outerTemplateDecls.push_back(CTD);
    }
    else if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("NestedTemplate")) {
      //Filter nodes in header files
      if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CTD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      nestedTemplateDecls.push_back(CTD);
    }
    else if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("TargetMethod")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      if (!MT->hasBody())
        return;
      
      //Find a suitable outer template class with nested template member
      if (outerTemplateDecls.empty() || nestedTemplateDecls.empty())
        return;
      
      size_t outerIdx = getrandom::getRandomIndex(outerTemplateDecls.size() - 1);
      size_t nestedIdx = getrandom::getRandomIndex(nestedTemplateDecls.size() - 1);
      
      const clang::ClassTemplateDecl* outerDecl = outerTemplateDecls[outerIdx];
      const clang::ClassTemplateDecl* nestedDecl = nestedTemplateDecls[nestedIdx];
      
      //Check if nestedDecl is actually nested within outerDecl
      const clang::CXXRecordDecl* outerRecord = outerDecl->getTemplatedDecl();
      const clang::CXXRecordDecl* nestedRecord = nestedDecl->getTemplatedDecl();
      
      //Simple check: nestedRecord should be within outerRecord's scope
      if (nestedRecord->getDeclContext() != outerRecord)
        return;
      
      //Get template parameter name from outer template
      std::string paramName;
      if (outerDecl->getTemplateParameters()->size() > 0) {
        paramName = outerDecl->getTemplateParameters()->getParam(0)->getNameAsString();
        if (paramName.empty()) {
          paramName = "T";
        }
      } else {
        paramName = "T";
      }
      
      //Perform mutation on the source code text by applying string replacement
      std::string insertion = "\n/*mut293*/" + outerRecord->getNameAsString() + "<" + paramName + ">::template " + nestedRecord->getNameAsString() + ";\n";
      
      //Insert at beginning of method body
      clang::Stmt* body = MT->getBody();
      clang::SourceLocation insertLoc = body->getBeginLoc().getLocWithOffset(1);
      
      //Replace the original AST node with the mutated one
      Rewrite.InsertText(insertLoc, insertion);
    }
}
  
void MutatorFrontendAction_293::MutatorASTConsumer_293::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher outerMatcher = classTemplateDecl().bind("OuterTemplate");
    DeclarationMatcher nestedMatcher = classTemplateDecl(isNested()).bind("NestedTemplate");
    DeclarationMatcher methodMatcher = cxxMethodDecl(isTemplate(), hasBody()).bind("TargetMethod");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(outerMatcher, &callback);
    matchFinder.addMatcher(nestedMatcher, &callback);
    matchFinder.addMatcher(methodMatcher, &callback);
    matchFinder.matchAST(Context);
}