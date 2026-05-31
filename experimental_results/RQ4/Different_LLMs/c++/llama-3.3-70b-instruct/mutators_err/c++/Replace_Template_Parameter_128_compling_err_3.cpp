//header file
#pragma once
#include "Mutator_base.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/ASTMatchers/ASTMatchFinder.h"

/**
 * Replace_Template_Parameter_128
 */ 
class MutatorFrontendAction_128 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(128)

private:
    class MutatorASTConsumer_128 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_128(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::Type *> definedTypes;
    };
};

//source file
#include "../include/Replace_Template_Parameter_128.h"
#include "clang/AST/Type.h"
#include "clang/AST/TemplateBase.h"

// ========================================================================================================
#define MUT128_OUTPUT 1

void MutatorFrontendAction_128::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::TemplateDecl>(("TemplateDecl"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto templateParams = MT->getTemplateParameters();
      if (templateParams) {
        for (auto param : *templateParams) {
          if (auto *TP = dyn_cast<TemplateTypeParmType>(param->getType())) {
            // Get all defined types in the program
            for (auto &type : definedTypes) {
              // Perform mutation on the source code text by applying string replacement
              std::string replacement = type->getAsString();
              std::string original = param->getNameAsString();
              auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                       MT->getSourceRange());
              size_t pos = declaration.find(original);
              if (pos != std::string::npos) {
                declaration.replace(pos, original.length(), replacement);
              }
              //Replace the original AST node with the mutated one
              Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
            }
          }
        }
      }
    } else if (auto *DT = Result.Nodes.getNodeAs<clang::Type>(("DefinedType"))) {
      // Record defined types in the program
      definedTypes.push_back(DT);
    }
}

void MutatorFrontendAction_128::MutatorASTConsumer_128::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("TemplateDecl");
    TypeMatcher definedTypeMatcher = type().bind("DefinedType");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(definedTypeMatcher, &callback);
    matchFinder.matchAST(Context);
}