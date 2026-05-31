//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Incomplete_Type_With_Pointer_To_Member_231
 */ 
class MutatorFrontendAction_231 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(231)

private:
    class MutatorASTConsumer_231 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_231(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> complete_types;
    };
};

//source file
#include "../include/Mutator_231.h"

// ========================================================================================================
#define MUT231_OUTPUT 1

void MutatorFrontendAction_231::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("TemplateDecl")) {
      //Filter nodes in header files
      if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TD->getLocation()))
        return;
      //Get the source code text of target node
      auto decl = TD->getTemplatedDecl();
      if (!decl) return;
      //Look for decltype expressions within the template
      std::vector<const clang::DecltypeType*> decltype_types;
      std::vector<const clang::Type*> incomplete_types;
      
      //Traverse the template to find decltype expressions
      auto traverse = [&](clang::Decl* D) {
        if (auto* FD = dyn_cast<FunctionDecl>(D)) {
          for (auto* P : FD->parameters()) {
            auto* T = P->getType().getTypePtr();
            if (auto* DT = dyn_cast<DecltypeType>(T)) {
              decltype_types.push_back(DT);
              //Check if the underlying type is incomplete
              auto* underlying = DT->getUnderlyingType().getTypePtr();
              if (underlying->isIncompleteType()) {
                incomplete_types.push_back(underlying);
              }
            }
          }
        }
      };
      
      traverse(decl);
      
      //If we found incomplete types in decltype, replace them
      if (!incomplete_types.empty()) {
        //Find a complete type to use
        const clang::CXXRecordDecl* replacement_type = nullptr;
        for (auto* CT : complete_types) {
          if (CT && CT->isCompleteDefinition()) {
            replacement_type = CT;
            break;
          }
        }
        
        //If no complete type exists, create a dummy struct
        if (!replacement_type) {
          //Create a dummy struct declaration
          std::string dummy_struct = "struct DummyStruct231 { int member; };";
          SourceLocation insert_loc = TD->getBeginLoc();
          Rewrite.InsertTextBefore(insert_loc, dummy_struct + "\n");
          //We'll use this dummy type
          replacement_type = nullptr; //Will be created by insertion
        }
        
        //Perform mutation on the source code text by applying string replacement
        //Replace the incomplete type declaration with a complete one
        for (auto* incomplete : incomplete_types) {
          if (auto* RD = incomplete->getAsCXXRecordDecl()) {
            if (RD->isCompleteDefinition()) continue;
            
            std::string new_decl;
            if (RD->isEnum()) {
              //Convert enum to struct
              new_decl = "struct " + RD->getNameAsString() + " { int member; };";
            } else {
              //Complete the class/struct
              new_decl = RD->getNameAsString() + " { int member; };";
              if (RD->isStruct()) new_decl = "struct " + new_decl;
              else if (RD->isClass()) new_decl = "class " + new_decl;
            }
            
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(RD->getSourceRange()), new_decl);
          }
        }
      }
    }
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("CompleteRecord")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      if (!RD->isCompleteDefinition()) return;
      //Record complete types for potential replacement
      complete_types.push_back(RD);
    }
}
  
void MutatorFrontendAction_231::MutatorASTConsumer_231::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher template_matcher = clang::ast_matchers::templateDecl().bind("TemplateDecl");
    DeclarationMatcher complete_matcher = clang::ast_matchers::cxxRecordDecl(clang::ast_matchers::isCompleteDefinition()).bind("CompleteRecord");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(template_matcher, &callback);
    matchFinder.addMatcher(complete_matcher, &callback);
    matchFinder.matchAST(Context);
}