//source file
#include "../include/Mutator_Duplicate_Incomplete_Nested_Struct_287.h"

// ========================================================================================================
#define MUT287_OUTPUT 1

void MutatorFrontendAction_287::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("StructDecl")) {
        //Filter nodes in header files
        if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(SD->getLocation()))
            return;
        if (!SD->isStruct() || !SD->isCompleteDefinition())
            return;
        //Record the node information to be used in the mutation process
        structDecls.push_back(SD);
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FieldDecl>("FieldDecl")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;
        fieldDecls.push_back(FD);
    } else if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecl")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        varDecls.push_back(VD);
    } else if (auto *OD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("OuterStruct")) {
        if (!OD || !Result.Context->getSourceManager().isWrittenInMainFile(OD->getLocation()))
            return;
        if (!OD->isStruct() || !OD->isCompleteDefinition())
            return;
        
        //Get the source code text of target node
        std::string outerStructText = stringutils::rangetoStr(*(Result.SourceManager), OD->getSourceRange());
        
        //Generate new struct name
        std::string newStructName;
        if (structDecls.size() > 0) {
            //Try to modify an existing struct name
            size_t idx = getrandom::getRandomIndex(structDecls.size() - 1);
            std::string baseName = structDecls[idx]->getNameAsString();
            newStructName = baseName + "_dup";
            //Check if name already exists (simple check)
            for (const auto* decl : structDecls) {
                if (decl->getNameAsString() == newStructName) {
                    newStructName = "MutatedStruct";
                    break;
                }
            }
        } else {
            newStructName = "MutatedStruct";
        }
        
        //Find a member type and name from seed
        std::string memberType;
        std::string memberName;
        if (fieldDecls.size() > 0) {
            size_t idx = getrandom::getRandomIndex(fieldDecls.size() - 1);
            const clang::FieldDecl* field = fieldDecls[idx];
            memberType = field->getType().getAsString();
            memberName = field->getNameAsString();
        } else if (varDecls.size() > 0) {
            size_t idx = getrandom::getRandomIndex(varDecls.size() - 1);
            const clang::VarDecl* var = varDecls[idx];
            memberType = var->getType().getAsString();
            memberName = var->getNameAsString();
        } else {
            //Fallback to int and a default name
            memberType = "int";
            memberName = "mut_member";
        }
        
        //Create incomplete nested struct declaration line
        std::string nestedDecl = "struct " + newStructName + ";";
        
        //Insert nested declaration into outer struct body
        size_t openBracePos = outerStructText.find('{');
        if (openBracePos != std::string::npos) {
            outerStructText.insert(openBracePos + 1, "\n    " + nestedDecl + "\n");
        }
        
        //Create valid struct definition after outer struct
        std::string validStructDef = "struct " + newStructName + " { " + memberType + " " + memberName + "; };";
        
        //Duplicate incomplete declaration before valid definition
        std::string insertion = "\n" + nestedDecl + "\n" + validStructDef;
        
        //Perform mutation on the source code text by applying string replacement
        std::string mutatedText = outerStructText + insertion;
        
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(OD->getSourceRange()), mutatedText);
    }
}
  
void MutatorFrontendAction_287::MutatorASTConsumer_287::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher structMatcher = cxxRecordDecl(isStruct()).bind("StructDecl");
    DeclarationMatcher fieldMatcher = fieldDecl().bind("FieldDecl");
    DeclarationMatcher varMatcher = varDecl().bind("VarDecl");
    DeclarationMatcher outerMatcher = cxxRecordDecl(isStruct(), isDefinition()).bind("OuterStruct");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(structMatcher, &callback);
    matchFinder.addMatcher(fieldMatcher, &callback);
    matchFinder.addMatcher(varMatcher, &callback);
    matchFinder.addMatcher(outerMatcher, &callback);
    matchFinder.matchAST(Context);
}