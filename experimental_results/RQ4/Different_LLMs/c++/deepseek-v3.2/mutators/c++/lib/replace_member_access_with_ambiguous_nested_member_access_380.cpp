//source file
#include "../include/Mutator_Replace_Member_Access_With_Ambiguous_Nested_Member_Access_380.h"

// ========================================================================================================
#define MUT380_OUTPUT 1

void MutatorFrontendAction_380::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("FieldDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      candidateFields.push_back(FD);
      std::string typeStr = FD->getType().getAsString();
      candidateTypes.push_back(typeStr);
    }
    else if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("CXXRecordDecl")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      if (!RD->isCompleteDefinition())
        return;
      if (RD->isStruct() || RD->isClass() || RD->isUnion()) {
        //Get the source code text of target node
        auto decls = RD->decls();
        std::vector<clang::FieldDecl*> fields;
        for (auto decl : decls) {
          if (auto FD = dyn_cast<clang::FieldDecl>(decl)) {
            fields.push_back(FD);
          }
        }
        if (fields.empty())
          return;
        
        //Select a field to duplicate
        size_t fieldIdx = getrandom::getRandomIndex(fields.size() - 1);
        clang::FieldDecl* targetField = fields[fieldIdx];
        std::string fieldName = targetField->getNameAsString();
        
        //Determine type for anonymous struct member
        std::string insertType;
        if (!candidateTypes.empty()) {
          size_t typeIdx = getrandom::getRandomIndex(candidateTypes.size() - 1);
          insertType = candidateTypes[typeIdx];
        } else {
          insertType = "int";
        }
        
        //Find insertion location (after the target field)
        clang::SourceLocation insertLoc = targetField->getEndLoc();
        //Perform mutation on the source code text by applying string replacement
        std::string insertText = " struct { " + insertType + " " + fieldName + "; };";
        insertText = "\n/*mut380*/" + insertText;
        //Replace the original AST node with the mutated one
        Rewrite.InsertTextAfterToken(insertLoc, insertText);
      }
    }
}
  
void MutatorFrontendAction_380::MutatorASTConsumer_380::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher fieldMatcher = fieldDecl().bind("FieldDecl");
    DeclarationMatcher recordMatcher = cxxRecordDecl(isDefinition()).bind("CXXRecordDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(fieldMatcher, &callback);
    matchFinder.addMatcher(recordMatcher, &callback);
    matchFinder.matchAST(Context);
}