//header file
#pragma once
#include "Mutator_base.h"

/**
 * Forward_Declared_Nested_Union_With_Transparent_Attribute_151
 */ 
class MutatorFrontendAction_151 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(151)

private:
    class MutatorASTConsumer_151 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_151(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/Forward_Declared_Nested_Union_With_Transparent_Attribute_151.h"

// ========================================================================================================
#define MUT151_OUTPUT 1

void MutatorFrontendAction_151::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("unionDecl")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      //Find the first field that is a struct
      for (const auto *field : MT->fields()) {
        QualType T = field->getType();
        if (const auto *RT = T->getAs<clang::RecordType>()) {
          if (RT->isStructureType()) {
            const clang::CXXRecordDecl *structDecl = cast<clang::CXXRecordDecl>(RT->getDecl());
            std::string structName = structDecl->getNameAsString();
            //Insert forward declaration of union S before the union
            SourceLocation loc = MT->getBeginLoc();
            std::string forwardDecl = "union " + structName + ";";
            Rewrite.InsertText(loc, forwardDecl + "\n", true, true);
            //Replace 'struct S' with 'union S' in the field
            SourceRange fieldRange = field->getSourceRange();
            std::string fieldTypeText = stringutils::rangetoStr(*Result.SourceManager, fieldRange);
            if (fieldTypeText.find("struct ") == 0) {
              std::string newFieldType = "union " + structName;
              Rewrite.ReplaceText(fieldRange, newFieldType);
            }
            //Add the transparent_union attribute to the union
            SourceLocation endLoc = MT->getEndLoc();
            std::string attr = " __attribute__ ((__transparent_union__))";
            Rewrite.InsertText(endLoc, attr, true, true);
            break; //Process only the first such field
          }
        }
      }
    }
}
  
void MutatorFrontendAction_151::MutatorASTConsumer_151::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isUnion(), has(fieldDecl(hasType(recordType(isStructureType()))))).bind("unionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}