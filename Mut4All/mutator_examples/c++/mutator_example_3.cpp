//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Using_Declaration_In_Derived_Class_3
 */ 
class MutatorFrontendAction_3 : public MutatorFrontendAction {
    public:
      MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(3)
    
    private:
      class MutatorASTConsumer_3 : public MutatorASTConsumer {
        public:
          MutatorASTConsumer_3(Rewriter &R) : TheRewriter(R) {}
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
          std::vector<const clang::CXXRecordDecl *> cur_classes;
      };
    };

//source file
#include "../include/insert_using_declaration_in_derived_class_3.h"

// ========================================================================================================
#define MUT3_OUTPUT 1

void MutatorFrontendAction_3::Callback::run(const MatchFinder::MatchResult &Result) {
  string acc_sep;
  //Check whether the matched AST node is the target node
  if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    //Filter nodes in header files
    if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   CL->getLocation()))
      return; 
    if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
      return;
    //Record the candidate existing class names for insertion
    cur_classes.push_back(CL);
  }

  //Check whether the matched AST node is the target node
  else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Derived")) {
    //Filter nodes in header files
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    //Get the source code text of target node
    auto decls = DL->decls();
    for (auto decl : decls) {
      if (decl->getIdentifierNamespace() ==
          Decl::IdentifierNamespace::IDNS_Using) {
      }
    }
    if (DL->isCompleteDefinition() == false)
      return;
    for (std::size_t j = 0; j < cur_classes.size(); ++j) {
      if (DL->isDerivedFrom(cur_classes[j])) {
        auto fields = cur_classes[j]->fields();
        std::vector<FieldDecl *> public_field;
        std::vector<FieldDecl *> protected_field;
        for (auto field : fields) {
          if (field->getAccess() == AccessSpecifier::AS_protected)
            protected_field.push_back(field);
          else if (field->getAccess() == AccessSpecifier::AS_public)
            public_field.push_back(field);
        }
        //Perform mutation on the source code text by applying string replacement
        if (!protected_field.empty()) {
          acc_sep += "\npublic:\n";
          string classname = cur_classes[j]->getNameAsString();
          for (auto field : protected_field) {
            string fieldname = field->getNameAsString();
            acc_sep += "using " + classname + " :: " + fieldname + ";\n";
          }
          llvm::outs() << acc_sep;
        }
        if (!public_field.empty()) {
          acc_sep += "protected:\n";
          string classname = cur_classes[j]->getNameAsString();
          for (auto field : public_field) {
            string fieldname = field->getNameAsString();
            acc_sep += "using " + classname + " :: " + fieldname + ";\n";
          }
        }
      }
    }
    acc_sep = "\n/*mut3*/" + acc_sep;
    //Replace the original AST node with the mutated one
    Rewrite.ReplaceText(DL->getEndLoc(), 0, acc_sep);
  }
}

void MutatorFrontendAction_3::MutatorASTConsumer_3::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define ASTMatchers to identify the target AST node for mutation
    //Find the derived classes to be inserted
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("Derived");
    //Find base class names for insertion
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}