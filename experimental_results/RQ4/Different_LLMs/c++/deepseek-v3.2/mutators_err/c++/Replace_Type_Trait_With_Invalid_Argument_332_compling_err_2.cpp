//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Type_Trait_With_Invalid_Argument_332
 */ 
class MutatorFrontendAction_332 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(332)

private:
    class MutatorASTConsumer_332 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_332(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> nonTypeIdentifiers;
    };
};

//source file
#include "../include/Mutator_Replace_Type_Trait_With_Invalid_Argument_332.h"

// ========================================================================================================
#define MUT332_OUTPUT 1

void MutatorFrontendAction_332::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TT = Result.Nodes.getNodeAs<clang::TemplateSpecializationType>("TypeTrait")) {
      //Filter nodes in header files
      if (!TT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TT->getLocStart()))
        return;
      
      //Check if it's a std:: type trait
      const clang::TemplateDecl* TD = TT->getTemplateName().getAsTemplateDecl();
      if (!TD) return;
      std::string templateName = TD->getNameAsString();
      if (templateName.find("add_") == std::string::npos && 
          templateName.find("is_") == std::string::npos &&
          templateName.find("remove_") == std::string::npos &&
          templateName.find("make_") == std::string::npos)
        return;
      
      //Get the source code text of target node
      SourceRange range = TT->getSourceRange();
      std::string originalText = stringutils::rangetoStr(*(Result.SourceManager), range);
      
      //Find the type argument position
      size_t templateStart = originalText.find("<");
      size_t templateEnd = originalText.rfind(">");
      if (templateStart == std::string::npos || templateEnd == std::string::npos) return;
      
      std::string typeArg = originalText.substr(templateStart + 1, templateEnd - templateStart - 1);
      
      //Collect non-type identifiers from the AST
      if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
        if (VD && Result.Context->getSourceManager().isWrittenInMainFile(VD->getBeginLoc())) {
          nonTypeIdentifiers.push_back(VD->getNameAsString());
        }
      }
      if (auto *ED = Result.Nodes.getNodeAs<clang::EnumConstantDecl>("EnumConstant")) {
        if (ED && Result.Context->getSourceManager().isWrittenInMainFile(ED->getBeginLoc())) {
          nonTypeIdentifiers.push_back(ED->getNameAsString());
        }
      }
      if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
        if (FD && Result.Context->getSourceManager().isWrittenInMainFile(FD->getBeginLoc())) {
          nonTypeIdentifiers.push_back(FD->getNameAsString());
        }
      }
      
      //Perform mutation on the source code text by applying string replacement
      if (!nonTypeIdentifiers.empty()) {
        size_t idx = getrandom::getRandomIndex(nonTypeIdentifiers.size() - 1);
        std::string newArg = nonTypeIdentifiers[idx];
        std::string mutatedText = originalText.substr(0, templateStart + 1) + 
                                  newArg + originalText.substr(templateEnd);
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(range, mutatedText);
      }
    }
}
  
void MutatorFrontendAction_332::MutatorASTConsumer_332::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = templateSpecializationType(
        hasTemplateArgument(0, refersToType(type()))
    ).bind("TypeTrait");
    
    DeclarationMatcher varMatcher = varDecl().bind("VarDecl");
    DeclarationMatcher enumMatcher = enumConstantDecl().bind("EnumConstant");
    DeclarationMatcher funcMatcher = functionDecl().bind("FunctionDecl");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(enumMatcher, &callback);
    matchFinder.addMatcher(funcMatcher, &callback);
    matchFinder.matchAST(Context);
}