//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Lambda_In_Decltype_With_Pack_Expansion_126
 */ 
class MutatorFrontendAction_126 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(126)

private:
    class MutatorASTConsumer_126 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_126(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateTypeParmDecl *> templatePacks;
    };
};

//source file
#include "../include/Mutator_Replace_Lambda_In_Decltype_With_Pack_Expansion_126.h"

// ========================================================================================================
#define MUT126_OUTPUT 1

void MutatorFrontendAction_126::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplatePack")) {
      //Filter nodes in header files
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      //Record template parameter packs
      if (TP->isParameterPack()) {
        templatePacks.push_back(TP);
      }
    }
    else if (auto *DT = Result.Nodes.getNodeAs<clang::DecltypeType>("Decltype")) {
      //Filter nodes in header files
      if (!DT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DT->getAsTagDecl()->getLocation()))
        return;
      //Check if decltype contains a lambda expression
      const clang::Type *UnderlyingType = DT->getUnderlyingType().getTypePtr();
      if (!UnderlyingType || !UnderlyingType->isRecordType())
        return;
      
      //Get the lambda expression node
      const clang::CXXRecordDecl *LambdaDecl = UnderlyingType->getAsCXXRecordDecl();
      if (!LambdaDecl || !LambdaDecl->isLambda())
        return;
      
      //Ensure there is at least one template parameter pack available
      if (templatePacks.empty())
        return;
      
      //Select a random template pack
      size_t packIndex = getrandom::getRandomIndex(templatePacks.size() - 1);
      const clang::TemplateTypeParmDecl *SelectedPack = templatePacks[packIndex];
      
      //Get the source code text of target node
      CharSourceRange decltypeRange = Result.SourceManager->getExpansionRange(DT->getAsTagDecl()->getSourceRange());
      std::string decltypeText = stringutils::rangetoStr(*(Result.SourceManager), decltypeRange);
      
      //Find the lambda within decltype text
      //We need to extract the lambda part and modify it
      //Since decltype text is like "decltype([] { return 0; })", we need to locate the lambda
      size_t lambdaStart = decltypeText.find('[');
      if (lambdaStart == std::string::npos)
        return;
      size_t lambdaEnd = decltypeText.find(')', lambdaStart);
      if (lambdaEnd == std::string::npos)
        return;
      
      std::string lambdaPart = decltypeText.substr(lambdaStart, lambdaEnd - lambdaStart);
      
      //Modify lambda to include parameter pack expansion
      //Insert parameter list with auto... args
      size_t captureEnd = lambdaPart.find(']');
      if (captureEnd == std::string::npos)
        return;
      
      std::string modifiedLambda = lambdaPart.substr(0, captureEnd + 1);
      modifiedLambda += "(auto... args)";
      
      //Find the lambda body (after ']' or after parameter list if already present)
      size_t bodyStart = lambdaPart.find('{', captureEnd);
      if (bodyStart == std::string::npos)
        return;
      
      std::string lambdaBody = lambdaPart.substr(bodyStart);
      modifiedLambda += lambdaBody;
      
      //Add invocation with pack expansion
      std::string packName = SelectedPack->getNameAsString();
      if (packName.empty()) {
        //If pack has no name, use the pack expansion syntax directly
        packName = SelectedPack->getDeclName().getAsString();
      }
      
      modifiedLambda += "(" + packName + "...)";
      
      //Replace the lambda part in decltype text
      std::string mutatedDecltype = decltypeText.substr(0, lambdaStart) + modifiedLambda + decltypeText.substr(lambdaEnd);
      
      //Perform mutation on the source code text by applying string replacement
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(decltypeRange, mutatedDecltype);
    }
}
  
void MutatorFrontendAction_126::MutatorASTConsumer_126::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    //Matcher for template parameter packs
    DeclarationMatcher packMatcher = templateTypeParmDecl().bind("TemplatePack");
    //Matcher for decltype specifiers
    TypeMatcher decltypeMatcher = decltypeType().bind("Decltype");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(packMatcher, &callback);
    matchFinder.addMatcher(decltypeMatcher, &callback);
    matchFinder.matchAST(Context);
}