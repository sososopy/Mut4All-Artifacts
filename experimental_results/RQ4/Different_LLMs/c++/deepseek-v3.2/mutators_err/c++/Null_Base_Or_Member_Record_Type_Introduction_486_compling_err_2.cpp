//header file
#pragma once
#include "Mutator_base.h"

/**
 * Null_Base_Or_Member_Record_Type_Introduction_486
 */ 
class MutatorFrontendAction_486 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(486)

private:
    class MutatorASTConsumer_486 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_486(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TypedefNameDecl*> typeAliases;
        std::vector<std::string> fundamentalTypes = {"int", "float", "double", "char", "bool", "long", "short", "unsigned"};
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT486_OUTPUT 1

void MutatorFrontendAction_486::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isCompleteDefinition())
        return;
      //Get the source code text of target node
      auto classSource = stringutils::rangetoStr(*(Result.SourceManager), MT->getSourceRange());
      
      // Check for base classes
      bool hasBaseClasses = MT->getNumBases() > 0;
      // Check for member fields
      bool hasMemberFields = false;
      for (auto decl : MT->decls()) {
        if (clang::isa<clang::FieldDecl>(decl)) {
          hasMemberFields = true;
          break;
        }
      }
      
      if (!hasBaseClasses && !hasMemberFields)
        return;
      
      // Collect existing type aliases
      for (auto decl : MT->decls()) {
        if (auto *typedefDecl = clang::dyn_cast<clang::TypedefNameDecl>(decl)) {
          typeAliases.push_back(typedefDecl);
        }
      }
      
      // Choose mutation target: base class or member field
      int choice = getrandom::getRandomIndex(1);
      std::string mutatedSource = classSource;
      
      if (choice == 0 && hasBaseClasses) {
        // Mutate a base class
        int baseIndex = getrandom::getRandomIndex(MT->getNumBases() - 1);
        auto base = MT->getBaseClasses()[baseIndex];
        auto baseType = base.getType();
        std::string baseTypeStr = stringutils::rangetoStr(*(Result.SourceManager), base.getSourceRange());
        
        // Find or create a non-record type alias
        std::string replacementType;
        bool needNewTypedef = false;
        
        for (auto alias : typeAliases) {
          auto aliasType = alias->getUnderlyingType();
          if (!aliasType->isStructureType() && !aliasType->isUnionType() && !aliasType->isClassType()) {
            replacementType = alias->getNameAsString();
            break;
          }
        }
        
        if (replacementType.empty()) {
          // No suitable alias found, create new typedef
          int typeIndex = getrandom::getRandomIndex(fundamentalTypes.size() - 1);
          replacementType = "NonRecordType_" + std::to_string(getrandom::getRandomIndex(1000));
          needNewTypedef = true;
          // Insert typedef before class definition
          std::string typedefStr = "typedef " + fundamentalTypes[typeIndex] + " " + replacementType + ";";
          Rewrite.InsertTextBefore(MT->getBeginLoc(), "/*mut486*/" + typedefStr + "\n");
        }
        
        // Replace base class type
        size_t pos = mutatedSource.find(baseTypeStr);
        if (pos != std::string::npos) {
          mutatedSource.replace(pos, baseTypeStr.length(), replacementType);
        }
      } else if (hasMemberFields) {
        // Mutate a member field
        std::vector<clang::FieldDecl*> fields;
        for (auto decl : MT->decls()) {
          if (auto *field = clang::dyn_cast<clang::FieldDecl>(decl)) {
            fields.push_back(field);
          }
        }
        int fieldIndex = getrandom::getRandomIndex(fields.size() - 1);
        auto field = fields[fieldIndex];
        auto fieldType = field->getType();
        std::string fieldTypeStr = stringutils::rangetoStr(*(Result.SourceManager), field->getSourceRange());
        
        // Find or create a non-record type alias
        std::string replacementType;
        bool needNewTypedef = false;
        
        for (auto alias : typeAliases) {
          auto aliasType = alias->getUnderlyingType();
          if (!aliasType->isStructureType() && !aliasType->isUnionType() && !aliasType->isClassType()) {
            replacementType = alias->getNameAsString();
            break;
          }
        }
        
        if (replacementType.empty()) {
          // No suitable alias found, create new typedef
          int typeIndex = getrandom::getRandomIndex(fundamentalTypes.size() - 1);
          replacementType = "NonRecordType_" + std::to_string(getrandom::getRandomIndex(1000));
          needNewTypedef = true;
          // Insert typedef before class definition
          std::string typedefStr = "typedef " + fundamentalTypes[typeIndex] + " " + replacementType + ";";
          Rewrite.InsertTextBefore(MT->getBeginLoc(), "/*mut486*/" + typedefStr + "\n");
        }
        
        // Replace field type
        size_t pos = mutatedSource.find(fieldTypeStr);
        if (pos != std::string::npos) {
          mutatedSource.replace(pos, fieldTypeStr.length(), replacementType);
        }
      }
      
      //Replace the original AST node with the mutated one
      mutatedSource = "/*mut486*/" + mutatedSource;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutated1Source);
    }
}
  
void MutatorFrontendAction_486::MutatorASTConsumer_486::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}