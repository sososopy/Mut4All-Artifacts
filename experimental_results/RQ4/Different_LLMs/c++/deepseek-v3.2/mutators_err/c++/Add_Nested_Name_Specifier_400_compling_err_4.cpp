//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Nested_Name_Specifier_400
 */ 
class MutatorFrontendAction_400 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(400)

private:
    class MutatorASTConsumer_400 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_400(Rewriter &R) : TheRewriter(R) {}
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
        bool mutated = false;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT400_OUTPUT 1

void MutatorFrontendAction_400::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("OutOfClassDef")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      // Ensure we only mutate once
      if (mutated) return;
      // Check it's an out-of-class definition of a member function
      if (!FD->isOutOfLine()) return;
      // Check it has a trailing requires clause
      const auto *TrailingRequires = FD->getTrailingRequiresClause();
      if (!TrailingRequires) return;
      // Ensure it's a member of a class template
      const auto *MD = dyn_cast<CXXMethodDecl>(FD);
      if (!MD) return;
      const auto *RD = MD->getParent();
      if (!RD || !RD->isTemplateDecl()) return;
      
      //Get the source code text of target node
      auto definition = stringutils::rangetoStr(*(Result.SourceManager),
                                                 FD->getSourceRange());
      // Find the position after the first '::' following the class name
      // The pattern is: return_type ClassName<TemplateArgs>::FunctionName() const requires (...)
      // We need to insert ClassName:: after that '::'
      std::string className = RD->getNameAsString();
      // Find "::" that separates class name from function name
      size_t classSepPos = definition.find("::");
      if (classSepPos == std::string::npos) return;
      // Ensure there's no nested specifier already (avoid double insertion)
      // Check if there's another "::" immediately after this one (skip whitespace)
      size_t afterSep = classSepPos + 2;
      while (afterSep < definition.size() && isspace(definition[afterSep])) ++afterSep;
      if (definition.substr(afterSep, className.size()+2) == className + "::") return;
      
      //Perform mutation on the source code text by applying string replacement
      definition.insert(classSepPos + 2, className + "::");
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), definition);
      mutated = true;
    }
}
  
void MutatorFrontendAction_400::MutatorASTConsumer_400::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(
        isOutOfLine(),
        hasTrailingRequiresClause(),
        ofClass(cxxRecordDecl(isTemplateDecl()))
    ).bind("OutOfClassDef");
    Callback callback(TheRewriter);
    matchFinder.addMat1cher(matcher, &callback);
    matchFinder.matchAST(Context);
}